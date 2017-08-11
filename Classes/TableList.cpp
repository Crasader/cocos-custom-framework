#include "TableList.h"
#include "TableListCell.h"

TableList::TableList()
{
    
}
TableList::~TableList()
{
    
}
TableList* TableList::create()
{
    
    return TableList::create(nullptr,Size(100, 100));
}
TableList* TableList::create(TableListDataSource* dataSource, Size size)
{
    TableList *tablelist = new (std::nothrow) TableList();
    tablelist->initWithViewSize(size);
    tablelist->autorelease();
    tablelist->setDataSource(dataSource);
    return tablelist;
}
bool TableList::initWithViewSize(Size size)
{
    bool status = init();
    if (status)
    {
        setContentSize(size);
        return true;
    }
    return false;
}
bool TableList::init()
{
    if (ScrollView::init())
    {
        setDirection(Direction::VERTICAL);
        _innerContainer->ignoreAnchorPointForPosition(true);
        _innerContainer->setAnchorPoint(Vec2(0, 1));
        _margin = 1;
        _vordering = FillDirection::vertical;
        _viewPos[0] = Vec2(0, 0);
        _viewPos[1] = Vec2(0, 0);
		m_nstatus = true;
        return true;
    }
    return false;
   
}
void TableList::setVerticalFillOrder(FillDirection order)
{
    if (_vordering != order)
    {
        _vordering = order;
        if (order==FillDirection::vertical)
        {
            setDirection(Direction::VERTICAL);
        }
        else
        {
            setDirection(Direction::HORIZONTAL);
        }
        
        if (!_cellsUsed.empty())
        {
            this->reloadData();
        }
    }
}
TableList::FillDirection TableList::getVerticalFillOrder()
{
    return _vordering;
}

void TableList::reloadData()
{
    _cellsUsed.clear();
    _cellsFreed.clear();
    _vCellsPositions.clear();
    _length = _dataSource->numberOfCellsInTableList(this);
    
    cordSizeMap();
    caculateCellPos();
    initViewBoarderPos();
    initCellPos();
}
void TableList::cordSizeMap()
{
    _sizeVec.clear();
    for (int i=0; i<_length; i++) {
        auto size = _dataSource->TableListCellSizeForIndex(this,i);
        _sizeVec.push_back(size);
    }
}
void TableList::caculateCellPos()
{
    _vCellsPositions.clear();
    if(_vordering==FillDirection::horizontal)  //如果是水平方向
    {
        float first = 0;
        for (int i=0; i< _length; i++) {
            float otherWidth=0;
            for (int j =0; j<i; j++) {
                otherWidth+=_sizeVec.at(j).width+ _margin;
            }
            float x = first+otherWidth;
            float y = 0;
            //依次计算出点  将其添加到数组_posMap中（_posMap一开始是map，后来改成了vector）
            _vCellsPositions.push_back(Vec2(x,y));
        }
        //计算_innerContainer的可移动范围
        float Width = _vCellsPositions.at(_length-1).x+_sizeVec.at(_length-1).width;
        float Height = getContentSize().height;
        setInnerContainerSize(Size(Width,Height));
    }
    else
    {
        float first = 0;
        for (int i=0; i< _length; i++) {
            float otherHeight=0;
            for (int j =0; j<=i; j++) {
                //因为Y方向下排列Cell，所以都是负数
                otherHeight-=(_sizeVec.at(j).height+ _margin);
            }
            float x = 0;
            float y = first+otherHeight;
            _vCellsPositions.push_back(Vec2(x,y));
        }
        float Width = getContentSize().width;
        float Height = fabs(_vCellsPositions.at(_length-1).y);
		setInnerContainerSize(Size(Width, Height));
    }
}
void TableList::initViewBoarderPos()
{
    _viewPos[0] = Vec2(0, 0);
    _viewPos[1] = Vec2(0, 0);
    if(_vordering==FillDirection::horizontal)
    {
        //左
        auto pos  = _innerContainer->convertToWorldSpace(Vec2(0,0));
        _viewPos[0] = pos;
        //右
        auto pos2 = _innerContainer->convertToWorldSpace(Vec2(getContentSize().width,0));
        _viewPos[1] = pos2;
    }
    else
    {
        //上
        auto pos = convertToWorldSpace(Vec2(0,getContentSize().height));
        _viewPos[0] = pos;
        //下
        auto pos2  = convertToWorldSpace(Vec2(0,0));
        _viewPos[1] = pos2;
    }
}
void TableList::checkOutOffSide()
{
    if (_vordering==FillDirection::horizontal) {
        if (!_cellsUsed.empty()) {
            auto cell = _cellsUsed.at(0);
            auto cellPos = _innerContainer->convertToWorldSpace(cell->getPosition());
            if (cellPos.x + _sizeVec.at(0).width  < _viewPos[0].x) {
                _cellsFreed.pushBack(cell);
                _innerContainer->removeChild(cell);
                _cellsUsed.erase(0);
                return;
                
            }
            if(_cellsUsed.empty())
                return;
            
            cell = _cellsUsed.back();
            cellPos = _innerContainer->convertToWorldSpace(cell->getPosition());
            if (cellPos.x  > _viewPos[1].x) {
                _cellsFreed.pushBack(cell);
                _innerContainer->removeChild(cell);
                _cellsUsed.erase(_cellsUsed.size()-1);
                return;
            }
        }
    }
    else
    {
        if (!_cellsUsed.empty()) {

            auto cell = _cellsUsed.at(0);
            auto cellPos = _innerContainer->convertToWorldSpace(cell->getPosition());
            if (cellPos.y - _sizeVec.at(cell->getIdx()).height > _viewPos[0].y) {
                
                _innerContainer->removeChild(cell);
                _cellsFreed.pushBack(cell);
                
                _cellsUsed.erase(0);
                return;
                
            }
            
            if(_cellsUsed.empty())
                return;
            
            cell = _cellsUsed.back();
            cellPos = _innerContainer->convertToWorldSpace(cell->getPosition());
            if (cellPos.y < _viewPos[1].y) {
                _innerContainer->removeChild(cell);
                _cellsFreed.pushBack(cell);
                _cellsUsed.erase(_cellsUsed.size()-1);
                return;
            }
        }
    }
}
void TableList::initCellPos()
{
    for (int i=0; i<_vCellsPositions.size(); i++) {
        auto nextPos = _vCellsPositions.at(i);
        nextPos = _innerContainer->convertToWorldSpace(nextPos);
        
        if(_vordering==FillDirection::horizontal)
        {
            //如果将要添加的Cell的位置在边界内，就一直往里添加
            if(nextPos.x < _vCellsPositions.at(1).x)              {
                auto newCell = _dataSource->TableListCellAtIndex(this,i);
                initCellProperty(newCell,i);  //这个方法是设置cell的位置以及一些其他设置
                _cellsUsed.pushBack(newCell);
                _innerContainer-> addChild(newCell);
            }
            else
            {
                break;
            }
        }
        else
        {
            if(nextPos.y > _vCellsPositions.at(1).y)
            {
                auto newCell = _dataSource->TableListCellAtIndex(this,i);
                initCellProperty(newCell,i);
                _cellsUsed.pushBack(newCell);
                _innerContainer-> addChild(newCell);
            }
            else
            {
                break;
            }
        }
        
        
    }
    
}
void TableList::initCellProperty(TableListCell* cell,int index)
{
    cell->setIdx(index);
    Vec2 point =  _vCellsPositions.at(index);
    cell->setPosition(point);
    
}
TableListCell *TableList::dequeueCell()
{
    TableListCell *cell;
    
    if (_cellsFreed.empty()) {
        cell = nullptr;
    } else {
        cell = _cellsFreed.at(0);
        cell->retain();
        _cellsFreed.erase(0);
        cell->autorelease();
    }
    return cell;
}
void TableList::scrollViewDidScroll()
{
   // ScrollView::scrollViewDidScroll();
	checkOutOffSide();
    if(_cellsUsed.empty())
        return;
    if(_vordering==FillDirection::horizontal)
    {
        auto cell = _cellsUsed.at(_cellsUsed.size()-1);
        if(cell->getIdx() < _length-1)//说明其后面还有cell
        {
            int index = cell->getIdx()+1;
            auto nextPos = _vCellsPositions.at(index);
            nextPos = _innerContainer->convertToWorldSpace(nextPos);
			if (nextPos.x - _sizeVec.at(index).width< _viewPos[1].x)
            {
                auto newCell = _dataSource->TableListCellAtIndex(this,index);
                initCellProperty(newCell,index);
                _cellsUsed.pushBack(newCell);
                _innerContainer-> addChild(newCell);
            }
        }
        
        cell = _cellsUsed.at(0);
        if(cell->getIdx() >0)//说明其前面还有cell
        {
            int index = cell->getIdx()-1;  //前面那个cell 的index
            auto nextPos = _vCellsPositions.at(index);
            nextPos = _innerContainer->convertToWorldSpace(nextPos);
            if(nextPos.x +_sizeVec.at(index).width > _viewPos[0].x)
            {
                auto newCell = _dataSource->TableListCellAtIndex(this,index);
                initCellProperty(newCell,index);
                _cellsUsed.insert(0, newCell);
                _innerContainer-> addChild(newCell);
            }
        }
    }
    else
    {
        auto cell = _cellsUsed.at(_cellsUsed.size()-1);
        if(cell->getIdx() < _length-1)//说明其后面还有cell
        {
            int index = cell->getIdx()+1;
            auto nextPos = _vCellsPositions.at(index);
            nextPos = _innerContainer->convertToWorldSpace(nextPos);
			if (nextPos.y + _sizeVec.at(index).height > _viewPos[1].y)
            {
                
                auto newCell = _dataSource->TableListCellAtIndex(this,index);
                initCellProperty(newCell,index);
                _innerContainer-> addChild(newCell);
                _cellsUsed.pushBack(newCell);
            }
        }
        
        cell = _cellsUsed.at(0);
        if(cell->getIdx() > 0)//说明其前面还有cell
        {
            int index = cell->getIdx()-1;
            auto nextPos = _vCellsPositions.at(index);
            nextPos = _innerContainer->convertToWorldSpace(nextPos);
            if(nextPos.y  -_sizeVec.at(index).height  < _viewPos[0].y)
            {
                auto newCell = _dataSource->TableListCellAtIndex(this,index);
                initCellProperty(newCell,index);
                _innerContainer-> addChild(newCell);
                _cellsUsed.insert(0, newCell);
                CCLOG("_cellsUsed size = %d  _cellsFreed size = %d",_cellsUsed.size(),_cellsFreed.size());
            }
        }
    }
}
bool TableList::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    bool status = ScrollView::onTouchBegan(touch, unusedEvent);
	status = m_nstatus&&status;
    return status;
    
}
void TableList::onTouchMoved(Touch *touch, Event *unusedEvent)
{
    ScrollView::onTouchMoved(touch, unusedEvent);
    scrollViewDidScroll();
}
void TableList::onTouchEnded(Touch *touch, Event *unusedEvent)
{
     ScrollView::onTouchEnded(touch, unusedEvent);
}
void TableList::onTouchCancelled(Touch *touch, Event *unusedEvent)
{
    ScrollView::onTouchCancelled(touch, unusedEvent);
}
void TableList::removeCellAtIndex(int idx)
{
    int cordI = -1;
    
    for (int i=0; i<_cellsUsed.size(); i++) {
        if(_cellsUsed.at(i)->getIdx() == idx)
        {
            cordI = i;
            continue;
        }
        //为之后的Cell调整idx
        if (cordI!=-1) {
            auto cell = _cellsUsed.at(i);
            cell->setIdx(cell->getIdx()-1);//之后的Cell的 idx依次前移
        }
    }
    if (cordI!=-1) {
        
        auto cell = _cellsUsed.at(cordI);
        _cellsUsed.erase(cordI);  //删除之后后面的会向前移动一位
        _cellsFreed.pushBack(cell);
        removeChild(cell);
        
        updateWithRemove(idx);
        
        for (int i= cordI; i<_cellsUsed.size(); i++) {
            auto cell = _cellsUsed.at(i);
            //auto label = (Label*) cell->getChildByTag(9);
            //label->setString(__String::createWithFormat("%d",cell->getIdx())->getCString());
            //因为这里定的是一个固定的时间，所以其速度不一定相同，删除一个大的cell移动的快，删除一个小的cell移动的慢，也可以固定速度，只要将距离求出来就可以了
            auto pos = _vCellsPositions.at(cell->getIdx());
            cell->runAction(MoveTo::create(0.3, pos));
        }
    }
}
void TableList::updateWithRemove(int idx)
{
    
    if (_cellsUsed.empty()) {
        float mv_x =0;
        //更新_posMap
        if (idx != _vCellsPositions.size()-1) {  //如果不是最后一个
            mv_x = _sizeVec.at(idx).width + _margin;
            
            
            //新增Cell
            int index = _cellsUsed.back()->getIdx()+1;
            float delt = 0;
            while(delt < mv_x) {
                if (index==_length-1) {
                    break;
                }
                
                auto newCell = _dataSource->TableListCellAtIndex(this,index);
                initCellProperty(newCell,index);
                newCell->setPosition(_vCellsPositions.at(index+1));//因为cellUsed中的index统一前移了一位，但是_posMap还没移动，所以这里用+1
                _cellsUsed.pushBack(newCell);
                _innerContainer->addChild(newCell);
                
                
                delt += _sizeVec.at(index+1).width+_margin;
                index++;
            }
            
            
            //更新位置
            for(int i=idx;i<_vCellsPositions.size()-1;i++)
            {
                auto newX = _vCellsPositions.at(i+1).x - mv_x;
                auto newPos = Vec2(newX,0);
                _vCellsPositions.at(i) = newPos;
            }
            //更新size
            for(int i=idx;i<_sizeVec.size()-1;i++)
            {
                _sizeVec.at(i) = _sizeVec.at(i+1);
            }
        }
        else
        {
            mv_x = _sizeVec.at(idx).width;
        }
        //删除最后一个元素删掉
        _vCellsPositions.pop_back();
        _sizeVec.pop_back();
       
        
        
        //更新Length
        _length--;
        
        //更新大小
        auto size = getInnerContainerSize();
        size.width -= mv_x;
        setInnerContainerSize(size);
        initViewBoarderPos();
        
    }
    else
    {
        //will updating .............
        float mv_y =0;
        //更新_posMap
        if (idx != _vCellsPositions.size()-1) {  //如果不是最后一个
            mv_y = _sizeVec.at(idx).height + _margin;
            
            
            //新增Cell
            int index = _cellsUsed.back()->getIdx()+1;
            float delt = 0;
            while(delt < mv_y) {
                if (index==_length-1) {
                    break;
                }
                
                
                log("index = %d ",index);
                log("cell insert -------------------%d",index);
                auto newCell = _dataSource->TableListCellAtIndex(this,index);
                initCellProperty(newCell,index);
                //newCell->setContentSize(_sizeVec[index+1]);
                newCell->setPosition(_vCellsPositions.at(index+1));//因为cellUsed中的index统一前移了一位，但是
                _cellsUsed.pushBack(newCell);
                _innerContainer->addChild(newCell);
                
                
                delt += _sizeVec.at(index+1).height+_margin;
                index++;
            }
            
            //更新位置
            for(int i=idx;i<_vCellsPositions.size()-1;i++)
            {
                auto newY = _vCellsPositions.at(i+1).y + mv_y;
                auto newPos = Vec2(0,newY);
                _vCellsPositions.at(i) = newPos;
            }
            
            //更新size
            for(int i=idx;i<_sizeVec.size()-1;i++)
            {
                _sizeVec.at(i) = _sizeVec.at(i+1);
            }
            
            
        }
        else
        {
            mv_y = _sizeVec.at(idx).height;
        }
        //删除最后一个元素删掉
        _vCellsPositions.pop_back();
        _sizeVec.pop_back();
        
        
        //更新Length
        _length--;
        
        //记录当前滚动结点的位置
        auto pos = getInnerContainerPosition();
        
        
        //更新大小
        auto size = getInnerContainerSize();
        size.height -= mv_y;
        setInnerContainerSize(size);
        initViewBoarderPos();
        setInnerContainerPosition(pos);
        
    }
    
}
void TableList::setScrollEnable(bool status)
{
	m_nstatus = status;
}
bool TableList::getScrollEnable()
{
	return m_nstatus;
}

void TableList::update(float dt)
{
	if (_autoScrolling){
		processAutoScrolling(dt);
		scrollViewDidScroll();
	}
}
