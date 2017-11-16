# ifndef __Actions_H_
# define __Actions_H_

#include "cocos2d.h"
USING_NS_CC;

#include "myActionInterval.h"

using namespace std;


enum ClickActionType
{
	eDefault,
};

class Actions
{
public :
	//闪烁 repeatCount = -1 为 forever
	static ActionInterval* Flashing(const float delay, GLubyte opacity = 100,const int repeatCount = -1);

	//上下浮动
	static ActionInterval* FloatingForever(const float delay, const Vec2& distance);

	//呼吸 （放大缩小）
	static ActionInterval* BreathingForever(const float delay, const float size);

	static ActionInterval* DelayRun(const float delay, FiniteTimeAction * _CallBack);

	//摇一摇
	static ActionInterval * shake(float duration, float angle);

	static ActionInterval * click();

	static void click(Node* node, const ClickActionType& clickAction);
	static void click(Node* node, const std::function<void()>& endRun);
	static void click(Node* node, const ClickActionType& clickAction = eDefault, const std::function<void()>& endRun = nullptr);

	static ActionInterval * RotateForever(const float delay);

	static ActionInterval * Parabola(float t,const Point& startPoint, const Point& endPoint, float height = 0, float angle = 60);
};


class TemporalAction{
protected:
	float duration = 0, time = 0;
//	Interpolation interpolation;
	bool reverse = false, began = false, complete = true;

public:
	virtual void setDuration(float duration);
	virtual void begin();
	virtual bool act(float delta);
	virtual void update(float percent) = 0;
	virtual void end();
	virtual bool isComplete(){ return complete; };
	virtual void restart() {
		time = 0;
		began = false;
		complete = false;
	}
};

class MoveToAction : public TemporalAction{
private:
	float startX, startY;
	float endX, endY;
	Vec2 curPos;
public:
	virtual void begin(float duration, const Vec2& beginPos, const Vec2& endPos);
	virtual void update(float percent);
	virtual Vec2 getCurPos();
};
# endif