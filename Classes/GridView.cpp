#include "GridView.h"


GridView::GridView()
{

}

GridView::~GridView()
{
	_gridView_items.clear();
}

GridView* GridView::create(int cols)
{
	GridView* widget = new (std::nothrow) GridView();
	if (widget && widget->init(cols))
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

bool GridView::init(int cols)
{
	if (!ListView::init())
	{
		return false;
	}

	_cols = cols;
	this->setGravity(ListView::Gravity::CENTER_VERTICAL);
	return true;
}

bool GridView::init(ListView* listview, int cols)
{
	if (!ListView::init())
	{
		return false;
	}
	
	_cols = cols;

	ListView::copyProperties(listview);
	//this->setContentSize(node->getContentSize());
	//setPosition(node->getPosition());
	//setAnchorPoint(node->getAnchorPoint());
	//setScaleX(node->getScaleX());
	//setScaleY(node->getScaleY());
	//setRotation(node->getRotation());
	//setColor(node->getColor());
	//setOpacity(node->getOpacity());
	this->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	this->setGravity(ListView::Gravity::CENTER_VERTICAL);
	
	return true;
}

void GridView::doGridLayout()
{

	int length = _gridView_items.size();
	for (int i = 0; i < length; i++)
	{
		auto layoutIndex = i / _cols;
		auto itemIndex = i % _cols;
		auto grid = getGridFormLayout(layoutIndex, itemIndex);
		auto _layout = ListView::getItem(layoutIndex);
		auto item = _gridView_items.at(i);
		
		//if (item->getPosition() != grid.origin){//位置有变
			if (item->getParent() != _layout){
				item->removeFromParent();
				_layout->addChild(item);
			}
		//}
		
// 		auto posLabel = EasyBuild::toLabel(item->getChildByName("pos"));
// 		posLabel->setString(StringUtils::format("pos:%d",i));

		item->setPositionX(grid.origin.x + grid.size.width / 2);
		item->setPositionY(grid.origin.y + grid.size.height / 2);
	}

	for (auto item : _items)//把没有元素的 layout 移除
	{
		if (item->getChildrenCount() == 0){
			ListView::removeChild(item);
			break;
		}
	}

	ListView::doLayout();
}

void GridView::setDefaultModelLayout(Size size)
{
	Layout* _layout = Layout::create();
	if (size.width == 0 || size.height == 0){
		size = this->getContentSize();
	}
	_layout->setContentSize(size);
	ListView::setItemModel(_layout);
}

void GridView::addItem(Widget* item)
{
	CCASSERT(_model != nullptr, "the _model is can' be null, please use setItemModel()");

	int index = _gridView_items.size() % _cols;
	if (index == 0){
		pushBackDefaultItem();
	}
	auto _getlayout = ListView::getItem(_items.size() - 1);
//	auto grid = getGridFormLayout(_items.size() - 1,index);
	_getlayout->addChild(item);
// 	item->setPositionX(grid.origin.x + grid.size.width / 2);
// 	item->setPositionY(grid.origin.y + grid.size.height / 2);

	_gridView_items.pushBack(item);
	doGridLayout();
}

void GridView::addItem(Widget* item, ssize_t index)
{
	CCASSERT(_model != nullptr, "the _model is can' be null, please use setItemModel()");

	int _yu = _gridView_items.size() % _cols;
	if (_yu == 0){
		pushBackDefaultItem();
	}
	auto layoutIndex = index / _cols;
	auto itemIndex = index % _cols;
	auto _layout = ListView::getItem(layoutIndex);
	_layout->addChild(item);
	_gridView_items.insert(index, item);

	
// 	auto indexLabel = EasyBuild::toLabel(item->getChildByName("index"));;
// 	indexLabel->setString(StringUtils::format("index:%d", (int)index));

	doGridLayout();

}

void GridView::swap(Widget* object1, Widget* object2)
{
	_gridView_items.swap(object1, object2);
	doGridLayout();
}

void GridView::swap(ssize_t index1, ssize_t index2)
{
	_gridView_items.swap(index1, index2);
	doGridLayout();
}

ssize_t GridView::getGridItemIndex(Widget* item)
{
	return _gridView_items.getIndex(item);
}

Widget* GridView::getGridItem(ssize_t index)
{
	auto layoutIndex = index / _cols;
	auto itemIndex = index % _cols;
	auto _layout = ListView::getItem(layoutIndex);
	auto childrneSize = _layout->getChildrenCount();
	if (childrneSize != 0){
		auto item = _layout->getChildren().at(itemIndex);
		return static_cast<Widget*>(item);
	}
	return nullptr;
}

cocos2d::Rect GridView::getGridFormLayout(int layoutIndex, int itemIndex)
{
	if (_items.empty())return Rect();

	auto _layout = ListView::getItem(layoutIndex);
	cocos2d::Size size = _layout->getContentSize();
	float width = size.width / _cols;
	float height = size.height;
	float x = itemIndex * width + itemIndex * spaceSize.width;
	float y = 0;
	cocos2d::Rect rect = cocos2d::Rect(x, y, width, height);
	return rect;
}

cocos2d::Rect GridView::getGrid(int index)
{
	if (_items.empty())return Rect();

	auto layoutIndex = index / _cols;
	auto itemIndex = index % _cols;

	return getGridFormLayout(layoutIndex, itemIndex);;
}

void GridView::removeLastGridItem()
{
	if (_items.empty())return;
	removeGridItem(_gridView_items.size() - 1);

// 	auto _layout = ListView::getItem(_items.size() - 1);
// 	auto childrneSize = _layout->getChildrenCount();
// 	if (childrneSize != 0){
// 		auto item = _layout->getChildren().at(childrneSize - 1);
// 		_layout->removeChild(item);
// 		_gridView_items.eraseObject(static_cast<Widget*>(item));
// 	}
// 	if (_layout->getChildrenCount() == 0){
// 		ListView::removeChild(_layout);
// 	}
// 	doLayout();
}

void GridView::removeGridItem(Widget* item)
{
	 
	auto _layout = item->getParent();
	_layout->removeChild(item);
	_gridView_items.eraseObject(item);
	
	doGridLayout();

// 	auto _index = _gridView_items.getIndex(item);
// 	removeGridItem(_index);
}

void GridView::removeGridItem(ssize_t index)
{
	auto item = getGridItem(index);
	removeGridItem(item);
// 	auto layoutIndex = index / _cols;
// 	auto itemIndex = index % _cols;
// 	auto _layout = ListView::getItem(layoutIndex);
// 	auto childrneSize = _layout->getChildrenCount();
// 	if (childrneSize != 0){
// 		auto item = _layout->getChildren().at(itemIndex);
// 		_layout->removeChild(item);
// 		_gridView_items.eraseObject(static_cast<Widget*>(item));
// 	}
// 
// 	if (_layout->getChildrenCount() == 0){
// 		ListView::removeChild(_layout);
// 	}
// 	doLayout();
}

void GridView::removeItem(ssize_t index)
{
	auto _layout = getItem(index);
	auto children = _layout->getChildren();
	for (auto child : children)
	{
		_gridView_items.eraseObject(static_cast<Widget*>(child));
	}

	ListView::removeItem(index);
}

void GridView::removeLastItem()
{
	removeItem(_items.size() - 1);
}

void GridView::removeAllItems()
{
	_gridView_items.clear();
	ListView::removeAllItems();
}

void GridView::setGravity(Gravity gravity)
{
	CCASSERT(gravity == ListView::Gravity::CENTER_VERTICAL, "In GridView ,the Gravity only use CENTER_VERTICAL");
	ListView::setGravity(gravity);

}

void GridView::setSpace(float x, float y)
{
	setItemsMargin(y);
	spaceSize.setSize(x, y);
}
