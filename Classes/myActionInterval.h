#ifndef __myActionInterval_H__
#define __myActionInterval_H__

#include <cocos2d.h>

USING_NS_CC;

enum 
{
	ePathDrawTag = 1000,
	eRotateCorrectTag,
};

/** @class ZOrder
*/
class ZOrder : public ActionInstant
{
public:

	static ZOrder * create(int zOrder);

	virtual void update(float time);
	virtual ZOrder* clone();
	virtual ZOrder* reverse();

CC_CONSTRUCTOR_ACCESS:
	ZOrder() : _zOrder(0){}
	virtual ~ZOrder(){}

	/** init the action */
	bool initZOrder(int zOrder);

protected:
	int _zOrder;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(ZOrder);
};

/** @class ZOrder
*/
class Vibration : public ActionInterval
{
public:
	static Vibration* create(float d, float strength);
	static Vibration* create(float d, Point strength);

	bool initWithDuration(float d, Point strength);

	virtual void startWithTarget(Node* target);
	virtual void update(float time);
	virtual void stop(void);

protected:
	Point _initial;
	Point _strength;
};




/**  Rotates a CCNode object to a certain angle around
 a certain rotation point by modifying it's rotation
 attribute and position.
 The direction will be decided by the shortest angle.
 */
class RotateAroundTo : public cocos2d::RotateTo
{ 
public:
    
    /** creates the action */
    static RotateAroundTo* create(float duration, float dstAngle, cocos2d::Vec2 rotationPoint);
    /** initializes the action */
    bool init(float duration, float dstAngle, cocos2d::Vec2 rotationPoint);
    
    virtual ~RotateAroundTo() {}
    
protected:
    
    virtual void startWithTarget(cocos2d::Node *target);
    
    virtual void update(float time);

private:

	cocos2d::Vec2 rotationPoint_;
	cocos2d::Vec2 startPosition_;

	float dstAngle_;
	float startAngle_;
	float diffAngle_;

};

/** Rotates a CCNode object clockwise around a certain
 rotation point a number of degrees by modiying its
 rotation attribute and position.
 */
class RotateAroundBy : public cocos2d::RotateBy
{
public:
    
    /** creates the action */
    static RotateAroundBy* create(float duration, float angle, cocos2d::Vec2 rotationPoint);
    /** initializes the action */
    bool init(float duration, float angle, cocos2d::Vec2 rotationPoint);
    
    virtual ~RotateAroundBy() {}
    
protected:
    
    virtual void startWithTarget(cocos2d::Node *target);
    
    virtual void update(float time);

private:

	cocos2d::Vec2 rotationPoint_;
	cocos2d::Vec2 startPosition_;

	float startAngle_;
	float angle_;

};

/**  Moves a CCNode object to a certain angle around
 a certain rotation point by modifying it's position.
 The direction will be decided by the shortest angle.
 */
class MoveAroundTo : public cocos2d::RotateTo
{ 
public:
    
    /** creates the action */
    static MoveAroundTo* create(float duration, float dstAngle, cocos2d::Vec2 rotationPoint);
    /** initializes the action */
    bool init(float duration, float dstAngle, cocos2d::Vec2 rotationPoint);
    
    virtual ~MoveAroundTo() {}
    
protected:
    
    virtual void startWithTarget(cocos2d::Node *target);
    
    virtual void update(float time);

private:

	cocos2d::Vec2 rotationPoint_;
	cocos2d::Vec2 startPosition_;

	float dstAngle_;
	float startAngle_;
	float diffAngle_;
};

/** Moves a CCNode object clockwise around a certain
 rotation point a number of degrees by modiying its
 position.
 */

class MoveAroundBy : public cocos2d::RotateBy
{
public:
    
    /** creates the action */
    static MoveAroundBy* create(float duration, float angle, cocos2d::Vec2 rotationPoint);
    /** initializes the action */
    bool init(float duration, float angle, cocos2d::Vec2 rotationPoint);
    
    virtual ~MoveAroundBy() {}
    
protected:
    
    virtual void startWithTarget(cocos2d::Node *target);
    
    virtual void update(float time);

private:

	cocos2d::Vec2 rotationPoint_;
	cocos2d::Vec2 startPosition_;

	float startAngle_;
	float angle_;
};

/**  Create fake effect of 3D rotation CCNode around X axis
 */
class FakeRotateX : public cocos2d::RotateTo
{
public:
    /** creates the action with default dept = 6 */
    static FakeRotateX* create(float duration, float startAngle, float dstAngle, float depth = 6.0f);
    /** initializes the action */
    bool init(float duration, float startAngle, float dstAngle, float depth);

    virtual ~FakeRotateX() {}

protected:

    virtual void startWithTarget(cocos2d::Node *target);

    virtual void update(float time);

private:

	float startAngle_;
	float dstAngle_;
	float diffAngle_;
	float radius_;
	float depth_;

	cocos2d::Sprite *target_;
};

/**  Create fake effect of 3D rotation CCNode around Y axis
 */
class FakeRotateY : public cocos2d::RotateTo
{
public:
    /** creates the action with default dept = 6 */
    static FakeRotateY* create(float duration, float startAngle, float dstAngle, float depth = 6.0f);
    /** initializes the action */
    bool init(float duration, float startAngle, float dstAngle, float depth);

    virtual ~FakeRotateY() {}

protected:

    virtual void startWithTarget(cocos2d::Node *target);

    virtual void update(float time);

private:

	float startAngle_;
	float dstAngle_;
	float diffAngle_;
	float radius_;
	float depth_;

	cocos2d::Sprite *target_;
};

/**  Create animation of digital counter
 */
class LabelCounter : public cocos2d::ActionInterval
{
public:
    /** creates the action with initial value = 0 */
    static LabelCounter* create(float duration, int finalValue, int initialValue = 0.0f);
    /** initializes the action */
    bool init(float duration, int finalValue, int initialValue);

    virtual ~LabelCounter() {}
    
    virtual void setBeforeText(std::string text);
    
    virtual void setAfterText(std::string text);

protected:

    virtual void startWithTarget(cocos2d::Node *target);

    virtual void update(float time);

    virtual void setValue(int value);

    cocos2d::Label *target_;
    
    std::string _beforeText;
    std::string _afterText;

private:

    int finalValue_;
    int initialValue_;

    int step_;
};

/**  Create animation of timer counter
 */
class LabelTimeCounter : public LabelCounter
{
public:
    /** creates the action with initial value = 0 */
    static LabelTimeCounter* create(float duration, int finalValue, int initialValue = 0.0f);
    /** initializes the action */
    bool init(float duration, int finalValue, int initialValue);

    virtual ~LabelTimeCounter() {}

protected:

    virtual void setValue(int value);

};



class  CircleAction : public ActionInterval
{
public:
	bool initWithDuration(float t, Point center, float radius, float rStartRadians = 0);
	virtual void startWithTarget(Node *target);
	virtual void update(float time);
public:
	static CircleAction *create(float t, Point center, float radius, float rStartRadians = 0);

protected:
	Point _pCenter;
	float _fRadius;
	float _fStartRadians;


	Vec2 _previousPosition;
};

class EllipseBy : public ActionInterval
{
public:
	static EllipseBy * create(float t, Point& centerPosition, float aLength, float bLength, float angle);
	bool initWithDuration(float t, Point& centerPosition, float aLength, float bLength, float angle);
	virtual void startWithTarget(Node *target);
	virtual void update(float time);
protected:

	Point _centerPosition;
	float _aLength;
	float _bLength;
	float _angle;

	Vec2 _previousPosition;

};


class  HelixAction : public ActionInterval
{
public:
	bool initWithDuration(float t, Point center, float minRadius, float maxRadius, float rStartRadians = 0);
	virtual void startWithTarget(Node *target);
	virtual void update(float time);
public:
	static HelixAction *create(float t, Point center, float minRadius, float maxRadius, float rStartRadians = 0);

protected:
	Point _fCenter;
	float _fMinRadius;
	float _fMaxRadius;
	float _fStartRadians;


	Vec2 _previousPosition;
	Vec2 _currentPosition;
};




CC_DEPRECATED_ATTRIBUTE class PathDraw : public ActionInterval
{
public:
	static PathDraw* create(Color4F color = Color4F(0, 0, 1, 1));

	DrawNode* getDrawNode()
	{
		return _pDrawNode;
	}
	virtual bool initWithColor(Color4F color);
	virtual PathDraw* clone() const override;
	virtual PathDraw* reverse(void)const override;
	virtual void startWithTarget(Node* target) override;

	virtual void step(float dt) override;
	virtual bool isDone(void) const override;

	virtual void stop();

	virtual void removeDrawNode();
CC_CONSTRUCTOR_ACCESS:
	PathDraw(){}
	virtual ~PathDraw();

protected:
	Vec2 _pLastPos;
	DrawNode* _pDrawNode = nullptr;
	Node* _Monitoring = nullptr;
	Color4F _color;
	int _index;
private:

	static std::unordered_map<Node*, int> _mGetIsContains;
	CC_DISALLOW_COPY_AND_ASSIGN(PathDraw);
};


//RotateCorrect  修正角度
class RotateCorrect : public ActionInterval
{
public:
	static RotateCorrect* create(float offsetAngle = 0);
	virtual RotateCorrect* clone() const override;
	virtual RotateCorrect* reverse(void) const override;
	virtual void startWithTarget(Node *target) override;

	virtual void step(float dt) override;
	virtual bool isDone(void) const override;

	virtual void setOffsetAngle(float offsetAngle){
		_offsetAngle = offsetAngle;
	}

CC_CONSTRUCTOR_ACCESS:
	RotateCorrect() {}
	virtual ~RotateCorrect() {}
	virtual bool initWithOffsetAngle(float offsetAngle = 0);


	Vec2 _startPosition;
	Vec2 _previousPosition;
	Vec2 _currentPosition;

	float _offsetAngle = 0;
private:
	CC_DISALLOW_COPY_AND_ASSIGN(RotateCorrect);
};


// RunAction
typedef std::function<void(float dt)> RunningFunc;
class RunAction : public ActionInterval{


public:
	static RunAction* create(const RunningFunc& run);
	virtual RunAction* clone() const override;
	virtual RunAction* reverse(void) const override;

	virtual void step(float dt) override;
	virtual bool isDone(void) const override;

CC_CONSTRUCTOR_ACCESS:
	RunAction() {}
	virtual ~RunAction() {}
	virtual bool initWithFunc(const RunningFunc& run);
	RunningFunc _run;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(RunAction);
};


#endif /* defined(__myActionInterval_H__) */
