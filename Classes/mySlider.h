#ifndef __mySlider_H__
#define __mySlider_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

class mySlider : public Slider
{
public:
	CREATE_FUNC(mySlider);
	virtual void copySlider(Widget* model);
	
	virtual Scale9Sprite* getProgressBarRenderer();

private:
	
};

#endif /* defined(__mySlider_H__) */
