#include "Actions.h"

ActionInterval* Actions::Flashing(const float delay, GLubyte opacity, const int repeatCount/* = -1*/)
{
	ActionInterval* action = nullptr;

	auto seq = Sequence::create(FadeTo::create(delay, opacity), FadeIn::create(delay), nullptr);
	if (repeatCount == -1){
		action = RepeatForever::create(seq);
	}
	else{
		action = Repeat::create(seq, repeatCount);
	}
	
	return action;
}

ActionInterval* Actions::FloatingForever(const float delay, const Vec2& distance)
{
	ActionInterval* action = nullptr;

	auto seq = Sequence::create(MoveBy::create(delay, distance), MoveBy::create(delay, -distance), nullptr);
 
	action = RepeatForever::create(seq);
	
	return action;
}

ActionInterval* Actions::BreathingForever(const float delay, const float size)
{
	ActionInterval* action = nullptr;
	
	auto seq = Sequence::create(ScaleTo::create(delay, size), ScaleTo::create(delay,1), nullptr);
	
	action = RepeatForever::create(seq);
	
	return action;
}

ActionInterval* Actions::DelayRun(const float delay, FiniteTimeAction * _CallBack)
{
	ActionInterval* action = nullptr;

	auto seq = Sequence::create(DelayTime::create(delay),_CallBack, nullptr);

	return seq;
}

ActionInterval * Actions::shake(float duration, float angle)
{
	auto nRotate1 = EaseSineIn::create(RotateBy::create(duration, angle));
	auto nRotate2 = EaseSineIn::create(RotateBy::create(duration * 2, -angle * 2));
	auto nRotate3 = RotateBy::create(duration * 2, angle * 2);
	auto nRotate4 = RotateBy::create(duration, -angle);
	auto nSeq = Sequence::create(nRotate1, nRotate2, nRotate3, nRotate4, nullptr);
	return nSeq;
}

ActionInterval * Actions::click()
{
	return Sequence::create(ScaleTo::create(0.1, 1.2f), ScaleTo::create(0.1, 1.0f), nullptr);
}

void Actions::click(Node* node,const ClickActionType& clickAction)
{
	click(node, clickAction,nullptr);
}

void Actions::click(Node* node, const std::function<void()>& endRun)
{
	click(node, eDefault, endRun);
}

void Actions::click(Node* node, const ClickActionType& clickAction /*= eDefault*/, const std::function<void()>& endRun /*= nullptr*/)
{
	FiniteTimeAction * action = nullptr;
	switch (clickAction)
	{
	case eDefault:
		action = click();
		break;
	default:
		break;
	}
	if (endRun){
		node->runAction(Sequence::create(action, CallFunc::create(endRun), nullptr));
	}
	else{
		node->runAction(Sequence::create(action, nullptr));
	}
}

ActionInterval * Actions::RotateForever(const float delay)
{
	auto nSeq = RepeatForever::create(Sequence::create(RotateTo::create(0, 0), RotateBy::create(delay, 360), nullptr));
	return nSeq;
}

ActionInterval * Actions::Parabola(float t, const Point& startPoint, const Point& endPoint, float height /*= 0*/, float angle /*= 60*/)
{
	// 把角度转换为弧度  
	float radian = angle*M_PI / 180.0;
	// 第一个控制点为抛物线左半弧的中点  
	float q1x = startPoint.x + (endPoint.x - startPoint.x) / 4.0;
	Point q1 = Point(q1x, height + startPoint.y + cos(radian)*q1x);
	// 第二个控制点为整个抛物线的中点  
	float q2x = startPoint.x + (endPoint.x - startPoint.x) / 2.0;
	Point q2 = Point(q2x, height + startPoint.y + cos(radian)*q2x);

	//曲线配置  
	ccBezierConfig cfg;
	cfg.controlPoint_1 = q1;
	cfg.controlPoint_2 = q2;
	cfg.endPosition = endPoint;
	//使用CCEaseInOut让曲线运动有一个由慢到快的变化，显得更自然  
	return EaseInOut::create(BezierTo::create(t, cfg), 1);
}







/////////////////////////////////////////////////////////////
void TemporalAction::setDuration(float duration)
{
	this->duration = duration;
	complete = false;
}

void TemporalAction::begin()
{

}

bool TemporalAction::act(float delta)
{
	if (complete) return true;
	
	if (!began) {
		begin();
		began = true;
	}
	time += delta;
	complete = time >= duration;
	float percent;
	if (complete)
		percent = 1;
	else {
		percent = time / duration;
		//if (interpolation != null) percent = interpolation.apply(percent);
	}
	update(reverse ? 1 - percent : percent);
	if (complete) end();
	return complete;
}

void TemporalAction::end()
{

}

void MoveToAction::begin(float duration, const Vec2& beginPos, const Vec2& endPos)
{
	this->duration = duration;
	this->complete = false;
	startX = beginPos.x;
	startY = beginPos.y;

	endX = endPos.x;
	endY = endPos.y;
}

void MoveToAction::update(float percent)
{
	//CCLOG("the percent %.02f", percent);
	float x = startX + (endX - startX) * percent;
	float y = startY + (endY - startY) * percent;
	curPos.set(x, y);
}

cocos2d::Vec2 MoveToAction::getCurPos()
{
	return curPos;
}
