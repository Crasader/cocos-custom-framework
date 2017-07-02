//**********************************************
//*											   *		
//* 匀速贝塞尔曲线，计算量较大，需要优化，慎用 *
//*											   *		
//**********************************************
#ifndef __FishPath_H__
#define __FishPath_H__


#include "cocos2d.h"    
USING_NS_CC;

class myBezierBy_uv : public ActionInterval
{
public:

	static myBezierBy_uv* create(float t, const ccBezierConfig& config, bool enableAngle = false, float offsetAngle = 0);

	virtual myBezierBy_uv* clone() const override;
	virtual myBezierBy_uv* reverse(void) const override;
	virtual void startWithTarget(Node *target) override;

	virtual void update(float time) override;
	virtual void changeRotation(float time);

	virtual void setOffsetAngle(float offsetAngle){
		_offsetAngle = offsetAngle;
	}
	virtual void setEnableAngle(bool enableAngle){
		_enableAngle = enableAngle;
	}
	virtual float getOffsetAngle(){ return _offsetAngle; }
	virtual bool getEnableAngle(){ return _enableAngle; }

CC_CONSTRUCTOR_ACCESS:
	myBezierBy_uv() {}
	virtual ~myBezierBy_uv() {}
	virtual bool initWithDuration(float t, const ccBezierConfig& c, bool enableAngle = false, float offsetAngle = 0);

protected:
	ccBezierConfig _config;
	Vec2 _startPosition;
	Vec2 _previousPosition;
	Vec2 _currentPosition;

	float curve_total_length = 0;
	float _offsetAngle = 0;
	bool _enableAngle = true;
private:
	CC_DISALLOW_COPY_AND_ASSIGN(myBezierBy_uv);
};


class myBezierTo_uv : public myBezierBy_uv
{
public:
	static myBezierTo_uv* create(float t, const ccBezierConfig& c, bool enableAngle = false, float offsetAngle = 0);

	virtual void startWithTarget(Node *target) override;
	virtual myBezierTo_uv* clone() const override;
	virtual myBezierTo_uv* reverse(void) const override;

CC_CONSTRUCTOR_ACCESS:
	myBezierTo_uv() {}
	virtual ~myBezierTo_uv() {}

	virtual bool initWithDuration(float t, const ccBezierConfig& c, bool enableAngle = false, float offsetAngle = 0);

protected:
	ccBezierConfig _toConfig;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(myBezierTo_uv);
};



#endif /* defined(__FishPath_H__) */
