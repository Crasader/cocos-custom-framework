#ifndef __TABLELIST_H__
#define __TABLELIST_H__
#include "ui/CocosGUI.h"
#include "TableListCell.h"
#include "ui/UIScrollView.h"
#include "ui/GUIExport.h"
#include <set>
#include <vector>
using namespace cocos2d::ui;
class TableList;

class  TableListDelegate
{
public:
	virtual void scrollViewDidZoom(cocos2d::ui::ScrollView* view) {};
    virtual void tableCellTouched(TableList* table, TableListCell* cell) = 0;
    virtual void tableCellHighlight(TableList* table, TableListCell* cell){};
    virtual void tableCellUnhighlight(TableList* table, TableListCell* cell){};
    virtual void tableCellWillRecycle(TableList* table, TableListCell* cell){};

};

class  TableListDataSource
{
public:
    virtual ~TableListDataSource() {}
    virtual Size TableListCellSizeForIndex(TableList *table, ssize_t idx) {
        return TableListCellSizeForTable(table);
    };
    virtual Size TableListCellSizeForTable(TableList *table) {
        return Size::ZERO;
    };
    virtual TableListCell* TableListCellAtIndex(TableList *table, ssize_t idx) = 0;
    virtual ssize_t numberOfCellsInTableList(TableList *table) = 0;

};

class TableList : public cocos2d::ui::ScrollView
{
public:
    enum class FillDirection
    {
        vertical = 0,
        horizontal = 1,
    };
    //CC_PROPERTY(float,_margin,Margin);
    TableList();
    virtual ~TableList();
    static TableList* create();
    static TableList* create(TableListDataSource* dataSource, Size size);
    bool initWithViewSize(Size size);
    bool init();
    TableListDataSource* getDataSource() { return _dataSource; }
    void setDataSource(TableListDataSource* source) { _dataSource = source; }
    TableListDelegate* getDelegate() { return _TableListDelegate; }
    void setDelegate(TableListDelegate* pDelegate) { _TableListDelegate = pDelegate; }
    void setVerticalFillOrder(FillDirection order);
    FillDirection getVerticalFillOrder();
    void reloadData();
    void cordSizeMap();
    void caculateCellPos();
    void initViewBoarderPos();
    void checkOutOffSide();
    void initCellPos();
    void initCellProperty(TableListCell* cell,int index);
    TableListCell *dequeueCell();
    
    virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchEnded(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchCancelled(Touch *touch, Event *unusedEvent) override;
    void removeCellAtIndex(int idx);
    void updateWithRemove(int idx);
	void setScrollEnable(bool status);
	bool getScrollEnable();

	virtual void update(float dt);
protected:
    virtual void scrollViewDidScroll();
protected:
    FillDirection _vordering;
    Vector<TableListCell*> _cellsUsed;
    Vector<TableListCell*> _cellsFreed;
    std::vector<Size> _sizeVec;
    std::vector<Vec2> _vCellsPositions;
    TableListDataSource* _dataSource;
    TableListDelegate* _TableListDelegate;
    float _margin;
    ssize_t _length;
    Vec2 _viewPos[2];
	bool m_nstatus;
};

#endif /* __TABLELIST_H__ */
