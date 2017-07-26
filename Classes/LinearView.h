#ifndef __LinearView_H__
#define __LinearView_H__

#include "PublicDefine.h"


class LinearView : public ListView
{

public:
	LinearView();
	virtual ~LinearView();
	
	CREATE_FUNC_PARAM_1(LinearView, int);
	CREATE_FUNC_PARAM_2(LinearView, ListView*, int);
	bool init(int cols);
	bool init(ListView* layout, int cols);


	virtual void doGridLayout();

	virtual void setDirection(Direction dir);
	virtual void setContentSize(const Size& contentSize);

	virtual void addItem(Widget* item);
	virtual void addItem(Widget* item, ssize_t index);
	virtual void insertItem(Widget* item, ssize_t index);
	

	virtual void swap(Widget* object1, Widget* object2);
	virtual void swap(ssize_t index1, ssize_t index2);


	virtual void removeLastGridItem();
	virtual void removeGridItem(Widget* item);
	virtual void removeGridItem(ssize_t index);
	virtual void removeAllItems();
	
	virtual ssize_t getGridItemIndex(Widget* item);
	virtual Widget* getGridItem(ssize_t index);

	virtual void setGravity(Gravity gravity);
	virtual void setSpace(float _space);//间隔

	virtual Vector<Widget*> get_LinearView_Items(){ return _LinearView_items; };
	virtual ssize_t get_LinearView_Items_Size(){ return _LinearView_items.size(); };

	virtual const Size& getGridSize();

	virtual int getCols(){ return _cols; };
	virtual void setCols(int rIcols);

	/**
	* 设置 item 是否能超出 gird  
	*/
	virtual void setCanBiggerThanGrid(bool bCan);;
	virtual bool getCanBiggerThanGrid(){ return _bCanBiggerGrid; };
	/**
	* 以 grid 为基础，item 相对 grid 的缩放大小
	*/
	virtual void setConstraintScale(float scale){ _fConstraintScale = scale; };
	virtual float getConstraintScale(){ return _fConstraintScale; };
protected:

	virtual void setDefaultItem();
	virtual void checkSize(Widget* item);
protected:
	
	int _cols;
	bool _bCanBiggerGrid = false;
	float _fConstraintScale = 1;
	Vector<Widget*> _LinearView_items;


};
#endif /* defined(__LinearView_H__) */
