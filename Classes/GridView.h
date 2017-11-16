#ifndef __GridView_H__
#define __GridView_H__

#include "PublicDefine.h"


class GridView : public ListView
{

public:
	GridView();
	virtual ~GridView();
	static GridView* create(int cols);
	CREATE_FUNC_PARAM_2(GridView, ListView*, int);
	bool init(int cols);
	bool init(ListView* layout, int cols);

	virtual void doGridLayout();
	virtual void setDefaultModelLayout(Size size = Size::ZERO);
	virtual void addItem(Widget* item);
	virtual void addItem(Widget* item, ssize_t index);

	virtual void swap(Widget* object1, Widget* object2);
	virtual void swap(ssize_t index1, ssize_t index2);


	virtual void removeLastGridItem();
	virtual void removeGridItem(Widget* item);
	virtual void removeGridItem(ssize_t index);
	virtual void removeAllItems();

	virtual void removeItem(ssize_t index);
	virtual void removeLastItem();

	virtual ssize_t getGridItemIndex(Widget* item);
	virtual Widget* getGridItem(ssize_t index);

	virtual void setGravity(Gravity gravity);
	virtual void setSpace(float x,float y);//间隔

	virtual Vector<Widget*> get_GridView_Items(){ return _gridView_items; };
	virtual ssize_t get_GridView_Items_Size(){ return _gridView_items.size(); };

	virtual int getCols(){ return _cols; };
	virtual void setCols(int rIcols){ _cols = rIcols; };
protected:
	virtual cocos2d::Rect getGridFormLayout(int layoutIndex,int itemIndex);
	virtual cocos2d::Rect getGrid(int index);
protected:
	
	int _cols;
	cocos2d::Size spaceSize;
	Vector<Widget*> _gridView_items;


};
#endif /* defined(__GridView_H__) */
