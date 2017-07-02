#ifndef __ChainView_H__
#define __ChainView_H__

#include "PublicDefine.h"

typedef std::function<void(Ref*, int currentItemIndex)> CurItemIndexOnChangeListener;
class ChainView : public ListView
{

public:
	ChainView();
	virtual ~ChainView();

	CREATE_FUNC(ChainView);

	virtual bool init();
	virtual void setDefaultModelLayout(Size size = Size::ZERO);
	virtual void setDirection(Direction dir);
	virtual void show();

	virtual void addItem(Widget* item);
	virtual void addItem(Widget* item, ssize_t index);
	virtual void addItem();
	virtual void setMagneticType(MagneticType magneticType);

	virtual ssize_t getCurrentItemIndex();
	virtual void setCurrentItemIndex(ssize_t index);
	virtual ssize_t getPreviousItemIndex();


	virtual void jumpToItem(ssize_t itemIndex, const Vec2& positionRatioInView, const Vec2& itemAnchorPoint);
	virtual void scrollToItem(Widget* item);
	virtual void scrollToItem(ssize_t itemIndex);
	virtual void scrollToItem(ssize_t itemIndex, float time);
	virtual void scrollToItem(ssize_t itemIndex, const Vec2& positionRatioInView, const Vec2& itemAnchorPoint);
	virtual void scrollToItem(ssize_t itemIndex, const Vec2& positionRatioInView, const Vec2& itemAnchorPoint, float timeInSec);
	virtual void startMagneticScroll();


	virtual void handleReleaseLogic(Touch *touch) override;
	virtual void handlePressLogic(Touch *touch) override;

	virtual void setAdsorb(bool isAdsorb);
	virtual bool getAdsorb(){ return _isAdsorb; };

	virtual void update(float dt);

	virtual void setOnChangeCurItemIndexListener(CurItemIndexOnChangeListener rCurItemIndexOnChangeListener);
	virtual void onCurItemIndexChange(int index);
protected:
protected:
	bool _isAdsorb = false;
	bool _isTouchBegin = false;
	bool _doBoundItem = false;
	ssize_t _currentItemIndex = -1;
	ssize_t _previousPageIndex = -1;
	CurItemIndexOnChangeListener _pCurItemIndexOnChangeListener = nullptr;
};
#endif /* defined(__ChainView_H__) */
