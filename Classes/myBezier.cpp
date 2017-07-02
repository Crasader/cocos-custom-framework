#include "myBezier.h"  


// Bezier cubic formula:
//    ((1 - t) + t)3 = 1 
// Expands to ...
//   (1 - t)3 + 3t(1-t)2 + 3t2(1 - t) + t3 = 1 
static inline float bezierat(float a, float b, float c, float d, float t)
{
	return (powf(1 - t, 3) * a +
		3 * t*(powf(1 - t, 2))*b +
		3 * powf(t, 2)*(1 - t)*c +
		powf(t, 3)*d);
}

//
// copyBezierBy
//

copyBezierBy* copyBezierBy::create(float t, const ccBezierConfig& c)
{
	
	copyBezierBy *ref = new (std::nothrow) copyBezierBy();
	if (ref && ref->initWithDuration(t, c))
	{
		ref->autorelease();
		return ref;
	}

	delete ref;
	return nullptr;
}

bool copyBezierBy::initWithDuration(float t, const ccBezierConfig& c)
{
	if (ActionInterval::initWithDuration(t))
	{
		_config = c;
		return true;
	}

	return false;
}

void copyBezierBy::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_previousPosition = _startPosition = target->getPosition();
}

copyBezierBy* copyBezierBy::clone() const
{
	// no copy constructor
	return copyBezierBy::create(_duration, _config);
}

void copyBezierBy::update(float time)
{
	if (_target)
	{
		float xa = 0;
		float xb = _config.controlPoint_1.x;
		float xc = _config.controlPoint_2.x;
		float xd = _config.endPosition.x;

		float ya = 0;
		float yb = _config.controlPoint_1.y;
		float yc = _config.controlPoint_2.y;
		float yd = _config.endPosition.y;

		float x = bezierat(xa, xb, xc, xd, time);
		float y = bezierat(ya, yb, yc, yd, time);

#if CC_ENABLE_STACKABLE_ACTIONS
		Vec2 currentPos = _target->getPosition();
		Vec2 diff = currentPos - _previousPosition;
		_startPosition = _startPosition + diff;

		Vec2 newPos = _startPosition + Vec2(x, y);
		_target->setPosition(newPos);

		_previousPosition = newPos;
#else
		_target->setPosition(_startPosition + Vec2(x, y));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
	}
}

copyBezierBy* copyBezierBy::reverse() const
{
	ccBezierConfig r;

	r.endPosition = -_config.endPosition;
	r.controlPoint_1 = _config.controlPoint_2 + (-_config.endPosition);
	r.controlPoint_2 = _config.controlPoint_1 + (-_config.endPosition);

	copyBezierBy *action = copyBezierBy::create(_duration, r);
	return action;
}





//
// myBezierBy
//


myBezierBy::myBezierBy()
{

}

myBezierBy::~myBezierBy()
{
	CC_SAFE_RELEASE_NULL(_points);
}

myBezierBy* myBezierBy::create(float t, ccBezierConfig& config, bool enableAngle /*= true*/, float offsetAngle /*= 0*/)
{

	PointArray* points = PointArray::create(4);
	points->addControlPoint(Vec2::ZERO);
	points->addControlPoint(config.controlPoint_1);
	points->addControlPoint(config.controlPoint_2);
	points->addControlPoint(config.endPosition);

	return create(t, points, enableAngle, offsetAngle);
}

myBezierBy* myBezierBy::create(float t, PointArray* points, bool enableAngle /*= true*/, float offsetAngle /*= 0*/)
{
	myBezierBy *ref = new (std::nothrow) myBezierBy();
	if (ref && ref->initWithDuration(t, points,enableAngle,offsetAngle))
	{
		ref->autorelease();
		return ref;
	}

	delete ref;
	return nullptr;
}


bool myBezierBy::initWithDuration(float t, PointArray* points, bool enableAngle /*= true*/, float offsetAngle /*= 0*/)
{
	if (ActionInterval::initWithDuration(t))
	{
		this->setPoints(points);
		this->setOffsetAngle(offsetAngle);
		this->setEnableAngle(enableAngle);

		return true;
	}

	return false;
}


myBezierBy* myBezierBy::clone() const
{
	// no copy constructor
	return myBezierBy::create(_duration, _points);
}

void myBezierBy::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_startPosition = _currentPosition = _previousPosition = target->getPosition();
}

myBezierBy* myBezierBy::reverse() const
{
	CCASSERT(false, "myBezierTo doesn't support the 'reverse' method");
	return nullptr;
}
void myBezierBy::update(float time)
{

	if (_target)
	{
// 		float xa = 0;
// 		float xb = _config.controlPoint_1.x;
// 		float xc = _config.controlPoint_2.x;
// 		float xd = _config.endPosition.x;
// 
// 		float ya = 0;
// 		float yb = _config.controlPoint_1.y;
// 		float yc = _config.controlPoint_2.y;
// 		float yd = _config.endPosition.y;

// 		float x = bezierat(xa, xb, xc, xd, time);
// 		float y = bezierat(ya, yb, yc, yd, time);

		Vec2 bezpos = p_bez(time, _points);
		float x = bezpos.x;
		float y = bezpos.y;

#if CC_ENABLE_STACKABLE_ACTIONS
		Vec2 currentPos = _target->getPosition();
		Vec2 diff = currentPos - _currentPosition;
		_startPosition = _startPosition + diff;
		
		Vec2 newPos = _startPosition + Vec2(x, y);
		_target->setPosition(newPos);

		_previousPosition = currentPos;
		_currentPosition = newPos;

		if(_enableAngle){
			double angle = atan2((_currentPosition.y - _previousPosition.y), (_currentPosition.x - _previousPosition.x));
			_target->setRotation(_offsetAngle - (angle * 360) / (2 * M_PI));
		}
 		
#else
		_target->setPosition(_startPosition + Vec2(x, y));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
	}
}

float myBezierBy::bez(int n, int k, float t)
{

	return c(n, k)*powf(t, k)*powf(1 - t, n - k);
}

float myBezierBy::c(int n, int k)
{
	int son = jiecheng(n);
	int mother = jiecheng(k)*jiecheng(n - k);
	return son / mother;
}

int myBezierBy::jiecheng(int i)
{
	int n = 1;
	for (int j = 1; j <= i; j++) {
		n *= j;
	}
	return n;
}

cocos2d::Vec2 myBezierBy::p_bez(float t, PointArray* sz)
{
	float x_p  = 0;
	float y_p = 0;
	int n = sz->count();
	for (int i = 0; i < n; i++) {
		float b  = bez(n - 1, i, t);
		x_p += sz->getControlPointAtIndex(i).x * b;
		y_p += sz->getControlPointAtIndex(i).y * b;
	}
	return Vec2(x_p, y_p);
}
