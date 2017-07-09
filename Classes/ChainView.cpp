#include "ChainView.h"

static const float OUT_OF_BOUNDARY_BREAKING_FACTOR = 0.05f;
ChainView::ChainView():
_isAdsorb(true)
{

}

ChainView::~ChainView()
{
	 
}

bool ChainView::init()
{
	if (!ListView::init())
	{
		return false;
	}

	this->setMagneticType(ListView::MagneticType::CENTER);
	this->setScrollBarEnabled(false);
	this->setItemsMargin(20);
	return true;
}

void ChainView::setMagneticType(MagneticType magneticType)
{
	CCASSERT((magneticType == ListView::MagneticType::CENTER), "the magneticType only can be CENTER");
	ListView::setMagneticType(magneticType);
}

void ChainView::addItem(Widget* item)
{
	pushBackCustomItem(item);
}

void ChainView::addItem(Widget* item, ssize_t index)
{
	insertCustomItem(item, index);
}


void ChainView::addItem()
{
	pushBackDefaultItem();
}

void ChainView::setDefaultModelLayout(Size size /*= Size::ZERO*/)
{
	Layout* _layout = Layout::create();
	if (size.width == 0 || size.height == 0){
		size = this->getContentSize();
	}
	_layout->setContentSize(size);
	ListView::setItemModel(_layout);
}

void ChainView::setDirection(Direction dir)
{
	switch (dir)
	{
	case cocos2d::ui::ScrollView::Direction::NONE:
	case cocos2d::ui::ScrollView::Direction::BOTH:
		break;
	case cocos2d::ui::ScrollView::Direction::VERTICAL:
		this->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
		break;
	case cocos2d::ui::ScrollView::Direction::HORIZONTAL:
		this->setGravity(ListView::Gravity::CENTER_VERTICAL);
		break;
	default:
		break;
	}

	ListView::setDirection(dir);
}

void ChainView::show()
{
	
	this->jumpToItem(0, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
}

void ChainView::handleReleaseLogic(Touch *touch)
{
	// Use `ScrollView` method in order to avoid `startMagneticScroll()` by `ListView`.
	ScrollView::handleReleaseLogic(touch);

	if (_items.empty())
	{
		return;
	}
	Vec2 touchMoveVelocity = flattenVectorByDirection(calculateTouchMoveVelocity());

	static const float INERTIA_THRESHOLD = 500;
	//DELOG(__FILE__, __LINE__, "touchMoveVelocity.length()  --- %f", touchMoveVelocity.length());
	if (touchMoveVelocity.length() < INERTIA_THRESHOLD)
	{
		startMagneticScroll();
	}
	else
	{
		_doBoundItem = true;
	}
}

void ChainView::handlePressLogic(Touch *touch)
{
	ListView::handlePressLogic(touch);
}

void ChainView::scrollToItem(ssize_t itemIndex)
{
	if (_innerContainerDoLayoutDirty) {
		this->forceDoLayout();
	}
	scrollToItem(itemIndex, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
}

void ChainView::scrollToItem(Widget* item)
{
	auto index = getIndex(item);
	scrollToItem(index);
}

void ChainView::scrollToItem(ssize_t itemIndex, float time)
{
	if (_innerContainerDoLayoutDirty) {
		this->forceDoLayout();
	}
	scrollToItem(itemIndex, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, time >= 0 ? time : _scrollTime);
}

void ChainView::scrollToItem(ssize_t itemIndex, const Vec2& positionRatioInView, const Vec2& itemAnchorPoint)
{
	scrollToItem(itemIndex, positionRatioInView, itemAnchorPoint,_scrollTime);
}

void ChainView::scrollToItem(ssize_t itemIndex, const Vec2& positionRatioInView, const Vec2& itemAnchorPoint, float timeInSec)
{
	//onCurItemIndexChange(itemIndex);
	ListView::scrollToItem(itemIndex, positionRatioInView, itemAnchorPoint, timeInSec);
}

void ChainView::setCurrentItemIndex(ssize_t index)
{
	jumpToItem(index, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
}

void ChainView::jumpToItem(ssize_t itemIndex, const Vec2& positionRatioInView, const Vec2& itemAnchorPoint)
{
	//onCurItemIndexChange(itemIndex);
	ListView::jumpToItem(itemIndex, positionRatioInView, itemAnchorPoint);
}

ssize_t ChainView::getCurrentItemIndex()
{
	//The _currentPageIndex is lazy calculated
	if (_innerContainerDoLayoutDirty) {
		_currentItemIndex = getIndex(getCenterItemInCurrentView());
	}
	return _currentItemIndex;
}


void ChainView::update(float dt)
{
	
	if (_autoScrolling){
		// Make auto scroll shorter if it needs to deaccelerate.
		float brakingFactor = (isNecessaryAutoScrollBrake() ? OUT_OF_BOUNDARY_BREAKING_FACTOR : 1);

		// Elapsed time
		_autoScrollAccumulatedTime += dt * (1 / brakingFactor);

		// Calculate the progress percentage
		float percentage = MIN(1, _autoScrollAccumulatedTime / _autoScrollTotalTime);
		if (_autoScrollAttenuate)
		{
			// Use quintic(5th degree) polynomial
			percentage = tweenfunc::quintEaseOut(percentage);
		}

		// Calculate the new position
		Vec2 newPosition = _autoScrollStartPosition + (_autoScrollTargetDelta * percentage);
		bool reachedEnd = std::abs(percentage - 1) <= this->getAutoScrollStopEpsilon();

		if (reachedEnd)
		{
			newPosition = _autoScrollStartPosition + _autoScrollTargetDelta;
		}

		if (_bounceEnabled)
		{
			// The new position is adjusted if out of boundary
			newPosition = _autoScrollBrakingStartPosition + (newPosition - _autoScrollBrakingStartPosition) * brakingFactor;
		}
		else
		{
			// Don't let go out of boundary
			Vec2 moveDelta = newPosition - getInnerContainerPosition();
			Vec2 outOfBoundary = getHowMuchOutOfBoundary(moveDelta);
			if (!fltEqualZero(outOfBoundary))
			{
				newPosition += outOfBoundary;
				reachedEnd = true;
			}
		}
		{
			Vec2 moveDelta = newPosition - getInnerContainerPosition();
			static const float MOVEDELTA_THRESHOLD = 5;
			if (_isAdsorb){
				if (moveDelta.length() <= MOVEDELTA_THRESHOLD){//移动的力减小到一定程度之后，对 item 进行束缚
					if (_doBoundItem){
						_doBoundItem = false;

						auto curIndex = getIndex(getCenterItemInCurrentView());
						curIndex = MIN(curIndex, _items.size() - 1);
						curIndex = MAX(curIndex, 0);
						scrollToItem(curIndex);
					}
				}
			}
		}
		// Finish auto scroll if it ended
		if (reachedEnd)
		{
			_autoScrolling = false;
			dispatchEvent(SCROLLVIEW_EVENT_AUTOSCROLL_ENDED, ScrollView::EventType::AUTOSCROLL_ENDED);
		}

		moveInnerContainer(newPosition - getInnerContainerPosition(), reachedEnd);
	}
	//DELOG(__FILE__, __LINE__, "_currentItemIndex %d", _currentItemIndex);
	updateScale();
}

void ChainView::onCurItemIndexChange(int index)
{
	if (_checkItemIndex == index)return;
	_checkItemIndex = index;
	_previousPageIndex = _currentItemIndex;
	_currentItemIndex = index;
	_checkItem = getItem(index);
//	DELOG(__FILE__, __LINE__, "onCurItemIndexChange _currentItemIndex -> %d", index);
	if (_pCurItemIndexOnChangeListener){
		_pCurItemIndexOnChangeListener(_checkItem, index);
	}
}

void ChainView::setOnChangeCurItemIndexListener(CurItemIndexOnChangeListener rCurItemIndexOnChangeListener)
{
	_pCurItemIndexOnChangeListener = rCurItemIndexOnChangeListener;
}



static Vec2 getAnchorPointByMagneticType(ListView::MagneticType magneticType)
{
	switch (magneticType)
	{
	case ListView::MagneticType::NONE: return Vec2::ZERO;
	case ListView::MagneticType::BOTH_END: return Vec2::ANCHOR_TOP_LEFT;
	case ListView::MagneticType::CENTER: return Vec2::ANCHOR_MIDDLE;
	case ListView::MagneticType::LEFT: return Vec2::ANCHOR_MIDDLE_LEFT;
	case ListView::MagneticType::RIGHT: return Vec2::ANCHOR_MIDDLE_RIGHT;
	case ListView::MagneticType::TOP: return Vec2::ANCHOR_MIDDLE_TOP;
	case ListView::MagneticType::BOTTOM: return Vec2::ANCHOR_MIDDLE_BOTTOM;
	}
	return Vec2::ZERO;
}

void ChainView::startMagneticScroll()
{
	if (_items.empty() || _magneticType == MagneticType::NONE)
	{
		return;
	}

	// Find the closest item
	Vec2 magneticAnchorPoint = getAnchorPointByMagneticType(_magneticType);
	Vec2 magneticPosition = -_innerContainer->getPosition();
	magneticPosition.x += getContentSize().width * magneticAnchorPoint.x;
	magneticPosition.y += getContentSize().height * magneticAnchorPoint.y;

	Widget* pTargetItem = getClosestItemToPosition(magneticPosition, magneticAnchorPoint);
	scrollToItem(getIndex(pTargetItem), magneticAnchorPoint, magneticAnchorPoint);
}

ssize_t ChainView::getPreviousItemIndex()
{
	return _previousPageIndex;
}

void ChainView::setAdsorb(bool isAdsorb)
{
	_isAdsorb = isAdsorb;
}

void ChainView::setMinScale(float _fMinScale)
{
	_minScale = _fMinScale;
}

void ChainView::updateScale()
{
	auto positionRatioInView = Vec2::ANCHOR_MIDDLE;
	Size contentSize = getContentSize();
	Vec2 targetPosition = -_innerContainer->getPosition();
	targetPosition.x += contentSize.width * positionRatioInView.x;
	targetPosition.y += contentSize.height * positionRatioInView.y;

	Vec2 centerPoint = targetPosition;
	float checkItemScale = MIN(_minScale+0.1f,1);
	
	for (int i = 0; i < _items.size();i++)
	{
		auto item = _items.at(i);
		auto pos = item->getPosition();
		auto differ = fabs(centerPoint.x - pos.x);
		auto centerRectSize = item->getContentSize().width;
		item->setScale(1 - (differ / centerRectSize * (1 - _minScale)));
		
		if (item->getScale() > checkItemScale){
			
				onCurItemIndexChange(i);
		
		}
	}
}
