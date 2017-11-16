#include "LinearView.h"


LinearView::LinearView()
{

}

LinearView::~LinearView()
{
	_LinearView_items.clear();
}

bool LinearView::init(int cols)
{
	if (!ListView::init())
	{
		return false;
	}

	_cols = cols;
	this->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);

	return true;
}

bool LinearView::init(ListView* listview, int cols)
{
	if (!ListView::init())
	{
		return false;
	}
	
	_cols = cols;

	ListView::copyProperties(listview);
	this->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);	

	return true;
}

void LinearView::setDefaultItem()
{
	Size selfSize = this->getContentSize();
	Size size;
	switch (_direction)
	{
	case cocos2d::ui::ScrollView::Direction::VERTICAL:
		size = Size(selfSize.width, selfSize.height/_cols);
		break;
	case cocos2d::ui::ScrollView::Direction::HORIZONTAL:
		size = Size(selfSize.width / _cols, selfSize.height);
		break;
	default:
		break;
	}


	Layout* _layout = Layout::create();
	_layout->setContentSize(size);
	ListView::setItemModel(_layout);
}

void LinearView::doGridLayout()
{
	for (auto item : _items)//把没有元素的 layout 移除
	{
		if (item->getChildrenCount() == 0){
			ListView::removeChild(item);
		}
	}
	ListView::doLayout();
}


void LinearView::addItem(Widget* item)
{
	CCASSERT(_model != nullptr, "the _model is can' be null, please use setItemModel()");

	pushBackDefaultItem();
	
	auto _getlayout = ListView::getItem(_items.size() - 1);
	checkSize(item);
	_getlayout->addChild(item);
	Vec2 itemCenter = item->getContentSize()/2;
	Vec2 layoutCenter = _getlayout->getContentSize() / 2;
	Vec2 pos = layoutCenter + (item->getAnchorPointInPoints() - itemCenter);
	item->setPosition(pos);
	_LinearView_items.pushBack(item);
	doGridLayout();
}

void LinearView::addItem(Widget* item, ssize_t index)
{
	CCASSERT(_model != nullptr, "the _model is can' be null, please use setItemModel()");

	auto _getlayout = _model->clone();
	_getlayout->addChild(item);
	Vec2 itemCenter = item->getContentSize() / 2;
	Vec2 layoutCenter = _getlayout->getContentSize() / 2;
	Vec2 pos = layoutCenter + (item->getAnchorPointInPoints() - itemCenter);
	item->setPosition(pos);
	_LinearView_items.insert(index, item);
	insertCustomItem(_getlayout, index);
	doGridLayout();
}

void LinearView::insertItem(Widget* item, ssize_t index)
{
	addItem(item,index);
}

void LinearView::swap(Widget* object1, Widget* object2)
{
	_LinearView_items.swap(object1, object2);
	_items.swap(object1, object2);
	onItemListChanged();
	requestDoLayout();
	doGridLayout();
}

void LinearView::swap(ssize_t index1, ssize_t index2)
{
	_LinearView_items.swap(index1, index2);
	_items.swap(index1, index2);
	onItemListChanged();
	requestDoLayout();
	doGridLayout();
}

ssize_t LinearView::getGridItemIndex(Widget* item)
{
	return _LinearView_items.getIndex(item);
}

Widget* LinearView::getGridItem(ssize_t index)
{
	 
	return _LinearView_items.at(index);
}

void LinearView::removeLastGridItem()
{
	if (_items.empty())return;
	removeGridItem(_LinearView_items.size() - 1);

}

void LinearView::removeGridItem(Widget* item)
{
	 
	auto _layout = item->getParent();
	ListView::removeChild(_layout);
	_LinearView_items.eraseObject(item);
	
	doGridLayout();
}

void LinearView::removeGridItem(ssize_t index)
{
	auto item = getGridItem(index);
	removeGridItem(item);
}

void LinearView::removeAllItems()
{
	_LinearView_items.clear();
	ListView::removeAllItems();
}

void LinearView::setGravity(Gravity gravity)
{
	ListView::setGravity(gravity);
}

void LinearView::setSpace(float _space)
{
	setItemsMargin(_space);
}

void LinearView::setDirection(Direction dir)
{
	ListView::setDirection(dir);
	switch (_direction)
	{
	case cocos2d::ui::ScrollView::Direction::VERTICAL:
		this->setGravity(ListView::Gravity::CENTER_VERTICAL);
		break;
	case cocos2d::ui::ScrollView::Direction::HORIZONTAL:
		this->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
		break;
	default:
		break;
	}

	setDefaultItem();
}

void LinearView::setContentSize(const Size& contentSize)
{
	ListView::setContentSize(contentSize);
	setDefaultItem();
}

void LinearView::setCols(int rIcols)
{
	_cols = rIcols;
	setDefaultItem();
}

const Size& LinearView::getGridSize()
{
	return _model->getContentSize();
}

void LinearView::setCanBiggerThanGrid(bool bCan)
{
	_bCanBiggerGrid = bCan;
}

void LinearView::checkSize(Widget* item)
{
	if (_bCanBiggerGrid){
		return;
	}
	auto gridSize = getGridSize();
	auto showpieceSprSize = item->getContentSize();
	//封装入 侧边栏
	if (showpieceSprSize.width > gridSize.width*_fConstraintScale || showpieceSprSize.height > gridSize.height*_fConstraintScale){
		auto scaleX = gridSize.width*_fConstraintScale / showpieceSprSize.width;
		auto scaleY = gridSize.height*_fConstraintScale / showpieceSprSize.height;
		auto scale = MIN(scaleX, scaleY);
		item->setScale(scale);
	}
}
