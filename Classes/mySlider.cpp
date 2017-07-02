#include "mySlider.h"

Scale9Sprite* mySlider::getProgressBarRenderer()
{
	return _progressBarRenderer;
}

void mySlider::copySlider(Widget* model)
{
	copyProperties(model);
}
