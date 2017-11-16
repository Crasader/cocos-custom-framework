#include "myBezier-uniform-velocity.h"
#include "PublicHead.h"

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

//坐标速度方程  
static inline float beze_speed_at(float a, float b, float c, float d, float t)
{
	double it = 1 - t;
	return -3 * a*it*it + 3 * b*it*it - 6 * b*it*t + 6 * c*it*t - 3 * c*t*t + 3 * d*t*t;
}
//速度方程  
static inline float beze_speed(const Point& a, const Point& b, const Point& c, const Point& d, float t)
{
	float sx = beze_speed_at(a.x, b.x, c.x, d.x, t);
	float sy = beze_speed_at(a.y, b.y, c.y, d.y, t);
	return sqrt(sx*sx + sy*sy);

}

//长度方程,使用Simpson积分算法  
static inline float beze_length(const Point& a, const Point& b, const Point& c, const Point& d, float t)
{
	//在总长度范围内，使用simpson算法的分割数  
#define TOTAL_SIMPSON_STEP  (10000)  

	//分割份数  
	int stepCounts = (int)(TOTAL_SIMPSON_STEP*t);
	if (stepCounts & 1) stepCounts++;    //偶数  
	if (stepCounts == 0) return 0.0;
	int halfCounts = stepCounts / 2;
	double sum1 = 0.0, sum2 = 0.0;
	double dStep = t / stepCounts;
	for (int i = 0; i < halfCounts; i++)
	{
		sum1 += beze_speed(a, b, c, d, (2 * i + 1)*dStep);
	}

	for (int i = 1; i < halfCounts; i++)
	{
		sum2 += beze_speed(a, b, c, d, (2 * i)*dStep);
	}
	return (beze_speed(a, b, c, d, 0.0f) + beze_speed(a, b, c, d, 1.0f) + 2 * sum2 + 4 * sum1)*dStep / 3.0;
}


//-------------------------------------------------------------------------------------  

//根据t推导出匀速运动自变量t'的方程(使用牛顿切线法)  
static inline float beze_even(const Point& a, const Point& b, const Point& c, const Point& d, float t, float total_length)
{
	float len = t * total_length; //如果按照匀速增长,此时对应的曲线长度  
	float t1 = t, t2;
	do
	{
		t2 = t1 - (beze_length(a, b, c, d, t1) - len) / beze_speed(a, b, c, d, t1);
		if (fabs(t1 - t2) <= 0.0001f) break;
		t1 = t2;
	} while (true);
	return t2;
}

//
// FishBezierBy
//
myBezierBy_uv* myBezierBy_uv::create(float t, const ccBezierConfig& config, bool enableAngle /*= false*/, float offsetAngle /*= 0*/)
{
	myBezierBy_uv *ref = new (std::nothrow) myBezierBy_uv();
	if (ref && ref->initWithDuration(t, config, enableAngle, offsetAngle))
	{
		ref->autorelease();
		return ref;
	}

	delete ref;
	return nullptr;
}

bool myBezierBy_uv::initWithDuration(float t, const ccBezierConfig& c, bool enableAngle /*= false*/, float offsetAngle /*= 0*/)
{
	if (ActionInterval::initWithDuration(t))
	{
		_config = c;
		this->setOffsetAngle(offsetAngle);
		this->setEnableAngle(enableAngle);

		return true;
	}

	return false;
}


void myBezierBy_uv::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_startPosition = _currentPosition = _previousPosition = target->getPosition();
	curve_total_length = beze_length(Point::ZERO, _config.controlPoint_1, _config.controlPoint_2, _config.endPosition, 1.0f);
}

myBezierBy_uv* myBezierBy_uv::clone() const
{
	// no copy constructor
	return myBezierBy_uv::create(_duration, _config,_enableAngle,_offsetAngle);
}

void myBezierBy_uv::update(float time)
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

		float pre = time;
		pre = beze_even(Point::ZERO, _config.controlPoint_1, _config.controlPoint_2, _config.endPosition, time, curve_total_length);//计算为匀速

		float x = bezierat(xa, xb, xc, xd, pre);
		float y = bezierat(ya, yb, yc, yd, pre);

#if CC_ENABLE_STACKABLE_ACTIONS
		Vec2 currentPos = _target->getPosition();
		Vec2 diff = currentPos - _currentPosition;
		_startPosition = _startPosition + diff;

		Vec2 newPos = _startPosition + Vec2(x, y);
		_target->setPosition(newPos);

		_previousPosition = currentPos;
		_currentPosition = newPos;

		if (_enableAngle){
			changeRotation(time);
		}
#else
		_target->setPosition(_startPosition + Vec2(x, y));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
	}
}

void myBezierBy_uv::changeRotation(float time)
{
	double angle = atan2((_currentPosition.y - _previousPosition.y), (_currentPosition.x - _previousPosition.x));
	float newRotation = _offsetAngle - (angle * 360) / (2 * M_PI);
	
	_target->setRotation(newRotation);

	//bool isleft = _currentPosition < _previousPosition;
	//_target->setScaleY(isleft ? -1 : 1);
	//_target->setScale(-1);
}

myBezierBy_uv* myBezierBy_uv::reverse() const
{
	ccBezierConfig r;

	r.endPosition = -_config.endPosition;
	r.controlPoint_1 = _config.controlPoint_2 + (-_config.endPosition);
	r.controlPoint_2 = _config.controlPoint_1 + (-_config.endPosition);

	myBezierBy_uv *action = myBezierBy_uv::create(_duration, r,_enableAngle,_offsetAngle);
	return action;
}

//
// FishBezierTo
//


myBezierTo_uv* myBezierTo_uv::create(float t, const ccBezierConfig& c, bool enableAngle /*= false*/, float offsetAngle /*= 0*/)
{
	myBezierTo_uv *ref = new (std::nothrow) myBezierTo_uv();
	if (ref && ref->initWithDuration(t, c, enableAngle, offsetAngle))
	{
		ref->autorelease();
		return ref;
	}

	delete ref;
	return nullptr;
}

bool myBezierTo_uv::initWithDuration(float t, const ccBezierConfig& c, bool enableAngle /*= false*/, float offsetAngle /*= 0*/)
{

	if (ActionInterval::initWithDuration(t))
	{
		_toConfig = c;
		this->setOffsetAngle(offsetAngle);
		this->setEnableAngle(enableAngle);

		return true;
	}
	return false;
}

myBezierTo_uv* myBezierTo_uv::clone() const
{
	return myBezierTo_uv::create(_duration, _toConfig, _enableAngle, _offsetAngle);
}

void myBezierTo_uv::startWithTarget(Node *target)
{

	ActionInterval::startWithTarget(target);
	_startPosition = _currentPosition = _previousPosition = target->getPosition();
	
	_config.controlPoint_1 = _toConfig.controlPoint_1 - _startPosition;
	_config.controlPoint_2 = _toConfig.controlPoint_2 - _startPosition;
	_config.endPosition = _toConfig.endPosition - _startPosition;
	

	curve_total_length = beze_length(Point::ZERO, _config.controlPoint_1, _config.controlPoint_2, _config.endPosition, 1.0f);
}

myBezierTo_uv* myBezierTo_uv::reverse() const
{
	CCASSERT(false, "CCFishBezierTo doesn't support the 'reverse' method");
	return nullptr;
}
