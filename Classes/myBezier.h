#ifndef __myBezier_h__  
#define __myBezier_h__  

#include "cocos2d.h"    
USING_NS_CC;

class copyBezierBy : public ActionInterval
{
public:

public:

	static copyBezierBy* create(float t, const ccBezierConfig& c);

	virtual copyBezierBy* clone() const override;
	virtual copyBezierBy* reverse(void) const override;
	virtual void startWithTarget(Node *target) override;

	virtual void update(float time) override;

CC_CONSTRUCTOR_ACCESS:
	copyBezierBy() {}
	virtual ~copyBezierBy() {}
	bool initWithDuration(float t, const ccBezierConfig& c);

protected:
	ccBezierConfig _config;
	Vec2 _startPosition;
	Vec2 _previousPosition;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(copyBezierBy);
};



class myBezierBy : public ActionInterval
{
public:

	static myBezierBy* create(float t, ccBezierConfig& config, bool enableAngle = true, float offsetAngle = 0);
	static myBezierBy* create(float t, PointArray* points, bool enableAngle = true, float offsetAngle = 0);


	virtual void startWithTarget(Node *target) override;
	virtual myBezierBy* clone() const override;
	virtual myBezierBy* reverse(void) const override;

	virtual void update(float time) override;

	virtual void setPoints(PointArray* points)
	{
		CC_SAFE_RETAIN(points);
		CC_SAFE_RELEASE(_points);
		_points = points;
	}
	virtual void setOffsetAngle(float offsetAngle){
		_offsetAngle = offsetAngle;
	}
	virtual void setEnableAngle(bool enableAngle){
		_enableAngle = enableAngle;
	}
	virtual float getOffsetAngle(){ return _offsetAngle; }
	virtual bool getEnableAngle(){ return _enableAngle; }
protected:
	virtual float bez(int n,int k,float t);
	virtual float c(int n,int k);
	virtual int jiecheng(int i);
	virtual Vec2 p_bez(float t,PointArray* sz);

CC_CONSTRUCTOR_ACCESS:
	myBezierBy();
	virtual ~myBezierBy();
	bool initWithDuration(float t, PointArray* points, bool enableAngle = true, float offsetAngle = 0);

protected:
	PointArray *_points = nullptr;
	Vec2 _startPosition;
	Vec2 _previousPosition;
	Vec2 _currentPosition;
	float _offsetAngle = 0;
	bool _enableAngle = true;
private:
	CC_DISALLOW_COPY_AND_ASSIGN(myBezierBy);
};



#endif // __myBezier_h__  