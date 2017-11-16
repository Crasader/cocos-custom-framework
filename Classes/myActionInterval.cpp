#include "myActionInterval.h"



//
// ZOrder Action
//
ZOrder * ZOrder::create(int zOrder)
{
	ZOrder *ret = new (std::nothrow) ZOrder();

	if (ret && ret->initZOrder(zOrder))
	{
		ret->autorelease();
	}

	return ret;
}

bool ZOrder::initZOrder(int zOrder)
{
	_zOrder = zOrder;
	return true;
}

void ZOrder::update(float /*time*/)
{
	_target->setLocalZOrder(_zOrder);
}

ZOrder *ZOrder::reverse()
{
	return ZOrder::create(_zOrder);
}

ZOrder * ZOrder::clone()
{
	// no copy constructor
	return ZOrder::create(_zOrder);
}



//
// Vibration
//
inline float rangedRand(float min, float max)
{
	return CCRANDOM_0_1() * (max - min) + min;
}

Vibration* Vibration::create(float d, float strength)
{
	return create(d, Point(strength, strength));
}

Vibration* Vibration::create(float duration, cocos2d::Point strength)
{
	Vibration* action = new Vibration();
	action->initWithDuration(duration, strength);
	action->autorelease();

	return action;
}

bool Vibration::initWithDuration(float duration, cocos2d::Point strength)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_strength = strength;
		return true;
	}

	return false;
}

void Vibration::update(float time)
{
	Point rand = Point(rangedRand(-_strength.x, _strength.x),
		rangedRand(-_strength.y, _strength.y));

	_target->setPosition(_initial + rand);
}

void Vibration::startWithTarget(Node* target)
{
	CCActionInterval::startWithTarget(target);

	_initial = target->getPosition();
}

void Vibration::stop(void)
{
	_target->setPosition(_initial);

	ActionInterval::stop();
}



//p'x = cos(theta) * (px-ox) - sin(theta) * (py-oy) + ox
//p'y = sin(theta) * (px-ox) + cos(theta) * (py-oy) + oy

// RotateAroundTo

RotateAroundTo* RotateAroundTo::create(float duration, float dstAngle, Vec2 rotationPoint)
{
    RotateAroundTo *action = new (std::nothrow) RotateAroundTo();
    action->autorelease();
    
    if (action->init(duration, dstAngle, rotationPoint) ) {
        return action;
    }
    
    return nullptr;
}

bool RotateAroundTo::init(float duration, float dstAngle, Vec2 rotationPoint)
{
    if (RotateTo::initWithDuration(duration, dstAngle, dstAngle)) {
        rotationPoint_ = rotationPoint;
        dstAngle_ = dstAngle;
        
        return true;
    }
    
    return false;
}

void RotateAroundTo::startWithTarget(Node *target)
{
    RotateTo::startWithTarget(target);
	startPosition_ = target->getPosition();
    startAngle_ = target->getRotation();
    calculateAngles(startAngle_, diffAngle_, dstAngle_);
}

void RotateAroundTo::update(float time)
{
    float x = cosf(CC_DEGREES_TO_RADIANS(-diffAngle_*time)) * ((startPosition_.x)-rotationPoint_.x) - sinf(CC_DEGREES_TO_RADIANS(-diffAngle_*time)) * ((startPosition_.y)-rotationPoint_.y) + rotationPoint_.x;
    float y = sinf(CC_DEGREES_TO_RADIANS(-diffAngle_*time)) * ((startPosition_.x)-rotationPoint_.x) + cosf(CC_DEGREES_TO_RADIANS(-diffAngle_*time)) * ((startPosition_.y)-rotationPoint_.y) + rotationPoint_.y;
    
    _target->setPosition(x, y);
    _target->setRotation( (startAngle_ + diffAngle_ * time ) );
}

// RotateAroundBy

RotateAroundBy* RotateAroundBy::create(float duration, float angle, Vec2 rotationPoint)
{
    RotateAroundBy *action = new (std::nothrow) RotateAroundBy();
    action->autorelease();
    
    if (action->init(duration, angle, rotationPoint) ) {
        return action;
    }
    
    return nullptr;
}

bool RotateAroundBy::init(float duration, float angle, Vec2 rotationPoint)
{
    if (RotateBy::initWithDuration(duration, angle, angle)) {
        rotationPoint_ = rotationPoint;
        angle_ = angle;
        
        return true;
    }
    
    return false;
}

void RotateAroundBy::startWithTarget(Node *target)
{
    RotateBy::startWithTarget(target);
    
    startPosition_ = target->getPosition();
    startAngle_ = target->getRotation();
}

void RotateAroundBy::update(float time)
{
    float x = cosf(CC_DEGREES_TO_RADIANS(-angle_*time)) * ((startPosition_.x)-rotationPoint_.x) - sinf(CC_DEGREES_TO_RADIANS(-angle_*time)) * ((startPosition_.y)-rotationPoint_.y) + rotationPoint_.x;
    float y = sinf(CC_DEGREES_TO_RADIANS(-angle_*time)) * ((startPosition_.x)-rotationPoint_.x) + cosf(CC_DEGREES_TO_RADIANS(-angle_*time)) * ((startPosition_.y)-rotationPoint_.y) + rotationPoint_.y;
    
    _target->setPosition(x, y);
    _target->setRotation( (startAngle_ + angle_ * time ) );
}

// MoveAroundTo

MoveAroundTo* MoveAroundTo::create(float duration, float dstAngle, Vec2 rotationPoint)
{
    MoveAroundTo *action = new (std::nothrow) MoveAroundTo();
    action->autorelease();
    
    if (action->init(duration, dstAngle, rotationPoint) ) {
        return action;
    }
    
    return nullptr;
}

bool MoveAroundTo::init(float duration, float dstAngle, Vec2 rotationPoint)
{
    if (RotateTo::initWithDuration(duration, dstAngle, dstAngle)) {
        rotationPoint_ = rotationPoint;
        dstAngle_ = dstAngle;
        
        return true;
    }
    
    return false;
}

void MoveAroundTo::startWithTarget(Node *target)
{
    RotateTo::startWithTarget(target);
	startPosition_ = target->getPosition();
    startAngle_ = target->getRotation();
    calculateAngles(startAngle_, diffAngle_, dstAngle_);
}

void MoveAroundTo::update(float time)
{
    float x = cosf(CC_DEGREES_TO_RADIANS(-diffAngle_*time)) * ((startPosition_.x)-rotationPoint_.x) - sinf(CC_DEGREES_TO_RADIANS(-diffAngle_*time)) * ((startPosition_.y)-rotationPoint_.y) + rotationPoint_.x;
    float y = sinf(CC_DEGREES_TO_RADIANS(-diffAngle_*time)) * ((startPosition_.x)-rotationPoint_.x) + cosf(CC_DEGREES_TO_RADIANS(-diffAngle_*time)) * ((startPosition_.y)-rotationPoint_.y) + rotationPoint_.y;
    
    _target->setPosition(x, y);
}

// MoveAroundBy

MoveAroundBy* MoveAroundBy::create(float duration, float angle, Vec2 rotationPoint)
{
    MoveAroundBy *action = new (std::nothrow) MoveAroundBy();
    action->autorelease();
    
    if (action->init(duration, angle, rotationPoint) ) {
        return action;
    }
    
    return nullptr;
}

bool MoveAroundBy::init(float duration, float angle, Vec2 rotationPoint)
{
    if (RotateBy::initWithDuration(duration, angle, angle)) {
        rotationPoint_ = rotationPoint;
        angle_ = angle;
        
        return true;
    }
    
    return false;
}

void MoveAroundBy::startWithTarget(Node *target)
{
    RotateBy::startWithTarget(target);
    
    startPosition_ = target->getPosition();
    startAngle_ = target->getRotation();
}

void MoveAroundBy::update(float time)
{
    float x = cosf(CC_DEGREES_TO_RADIANS(-angle_*time)) * ((startPosition_.x)-rotationPoint_.x) - sinf(CC_DEGREES_TO_RADIANS(-angle_*time)) * ((startPosition_.y)-rotationPoint_.y) + rotationPoint_.x;
    float y = sinf(CC_DEGREES_TO_RADIANS(-angle_*time)) * ((startPosition_.x)-rotationPoint_.x) + cosf(CC_DEGREES_TO_RADIANS(-angle_*time)) * ((startPosition_.y)-rotationPoint_.y) + rotationPoint_.y;
    
    _target->setPosition(x, y);
}

// FakeRotateX

FakeRotateX* FakeRotateX::create(float duration, float startAngle, float dstAngle, float depth/* = 6.0f*/)
{
    FakeRotateX *action = new (std::nothrow) FakeRotateX();
    action->autorelease();

    if (action->init(duration, startAngle, dstAngle, depth) ) {
        return action;
    }

    return nullptr;
}

bool FakeRotateX::init(float duration, float startAngle, float dstAngle, float depth)
{
    if (RotateTo::initWithDuration(duration, dstAngle, dstAngle)) {
        startAngle_ = startAngle;
        dstAngle_ = dstAngle;
        depth_ = depth;

        target_ = nullptr;

        return true;
    }

    return false;
}

void FakeRotateX::startWithTarget(Node *target)
{
    RotateTo::startWithTarget(target);

    radius_ = target->getContentSize().height / 2.0f;

    calculateAngles(startAngle_, diffAngle_, dstAngle_);

    target_ = dynamic_cast<Sprite *>(target); //dynamic_cast maybe overhead, but this action works only with sprites. todo: find better solution

    CCASSERT(target_, "this action can be used only with sprite");
}

void FakeRotateX::update(float time)
{
    PolygonInfo pi = target_->getPolygonInfo();

	pi.triangles.verts[0].vertices.y = cosf(-CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_ + radius_;
    pi.triangles.verts[0].vertices.x = ( sinf(-CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_) / depth_;

    pi.triangles.verts[1].vertices.y = cosf(M_PI - CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_ + radius_;
    pi.triangles.verts[1].vertices.x = ( sinf(M_PI - CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_ ) / depth_;

    pi.triangles.verts[2].vertices.y = cosf(CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_ + radius_;
    pi.triangles.verts[2].vertices.x = ( sinf(CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_ ) / depth_ + target_->getContentSize().width;

    pi.triangles.verts[3].vertices.y = cosf(M_PI + CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_ + radius_;
    pi.triangles.verts[3].vertices.x = ( sinf(M_PI + CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_) / depth_ + target_->getContentSize().width;

    target_->setPolygonInfo(pi);
}

// FakeRotateY

FakeRotateY* FakeRotateY::create(float duration, float startAngle, float dstAngle, float depth/* = 6.0f*/)
{
    FakeRotateY *action = new (std::nothrow) FakeRotateY();
    action->autorelease();

    if (action->init(duration, startAngle, dstAngle, depth) ) {
        return action;
    }

    return nullptr;
}

bool FakeRotateY::init(float duration, float startAngle, float dstAngle, float depth)
{
    if (RotateTo::initWithDuration(duration, dstAngle, dstAngle)) {
        startAngle_ = startAngle;
        dstAngle_ = dstAngle;
        depth_ = depth;

        target_ = nullptr;

        return true;
    }

    return false;
}

void FakeRotateY::startWithTarget(Node *target)
{
    RotateTo::startWithTarget(target);

    radius_ = target->getContentSize().width / 2.0;

    calculateAngles(startAngle_, diffAngle_, dstAngle_);

    target_ = dynamic_cast<Sprite *>(target); //dynamic_cast maybe overhead, but this action works only with sprites. todo: find better solution

    CCASSERT(target_, "this action can be used only with sprite");
}

void FakeRotateY::update(float time)
{
    PolygonInfo pi = target_->getPolygonInfo();

    pi.triangles.verts[0].vertices.x = cosf(M_PI + CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_ + radius_;
    pi.triangles.verts[0].vertices.y = ( sinf(M_PI + CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_) / depth_ + target_->getContentSize().height;

    pi.triangles.verts[1].vertices.x = cosf(M_PI - CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_ + radius_;
    pi.triangles.verts[1].vertices.y = ( sinf(M_PI - CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_ ) / depth_;

    pi.triangles.verts[2].vertices.x = cosf(CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_ + radius_;
    pi.triangles.verts[2].vertices.y = ( sinf(CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_ ) / depth_ + target_->getContentSize().height;

    pi.triangles.verts[3].vertices.x = cosf(-CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_ + radius_;
    pi.triangles.verts[3].vertices.y = ( sinf(-CC_DEGREES_TO_RADIANS(startAngle_ + diffAngle_*time) ) * radius_) / depth_;

    target_->setPolygonInfo(pi);
}

// LabelCounter

LabelCounter* LabelCounter::create(float duration, int finalValue, int initialValue)
{
    LabelCounter *action = new (std::nothrow) LabelCounter();
    action->autorelease();

    if (action->init(duration, finalValue, initialValue) ) {
        return action;
    }

    return nullptr;
}

bool LabelCounter::init(float duration, int finalValue, int initialValue)
{
    if (ActionInterval::initWithDuration(duration)) {
        finalValue_ = finalValue;
        initialValue_ = initialValue;

        target_ = nullptr;
        
        _beforeText = "";
        _afterText = "";

        return true;
    }

    return false;
}

void LabelCounter::setBeforeText(std::string text)
{
    _beforeText = text;
}

void LabelCounter::setAfterText(std::string text)
{
    _afterText = text;
}

void LabelCounter::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);

    step_ = finalValue_ - initialValue_;

    target_ = dynamic_cast<Label*>(target); //dynamic_cast maybe overhead, but this action works only with labels. todo: find better solution

    CCASSERT(target_, "this action can be used only with label");

    this->setValue(initialValue_);
}

void LabelCounter::update(float time)
{
    int value = initialValue_ + step_ * time;

    this->setValue(value);
}

void LabelCounter::setValue(int value)
{
    target_->setString(StringUtils::format("%s%i%s", _beforeText.c_str(), value, _afterText.c_str()));
}

// LabelTimeCounter

LabelTimeCounter* LabelTimeCounter::create(float duration, int finalValue, int initialValue)
{
    LabelTimeCounter *action = new (std::nothrow) LabelTimeCounter();
    action->autorelease();

    if (action->init(duration, finalValue, initialValue) ) {
        return action;
    }

    return nullptr;
}

bool LabelTimeCounter::init(float duration, int finalValue, int initialValue)
{
    if (LabelCounter::init(duration, finalValue, initialValue)) {
        return true;
    }

    return false;
}

void LabelTimeCounter::setValue(int value)
{
    target_->setString(StringUtils::format("%s%02i:%02i:%02i%s", _beforeText.c_str(), value / 3600, value % 3600 / 60, value % 60, _afterText.c_str()));
}


//CircleAction 圆形运动
CircleAction* CircleAction::create(float t, Point center, float radius, float rStartRadians/* = 0*/)
{
	CircleAction *pc = new CircleAction();
	pc->initWithDuration(t, center, radius, rStartRadians);
	pc->autorelease();

	return pc;
}

bool CircleAction::initWithDuration(float t, Point center, float radius, float rStartRadians/* = 0*/)
{
	if (ActionInterval::initWithDuration(t))
	{
		_pCenter = center;
		_fRadius = radius;
		_fStartRadians = rStartRadians;
		return true;
	}

	return false;
}


void CircleAction::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_previousPosition = target->getPosition();
}


void CircleAction::update(float time)
{
	if (_target)
	{
		Point last = _previousPosition;
		Point current = _target->getPosition();

		if (last.x != -500 || last.y != -500) {
			double angle = atan2((current.y - last.y), (current.x - last.x));
			_target->setRotation(90 - (angle * 360) / (2 * M_PI));
		}

		_previousPosition = current;
		_target->setPosition(_pCenter + Vec2(_fRadius*cos(_fStartRadians + (2 * M_PI)*time), _fRadius*sin(_fStartRadians + (2 * M_PI)*time)));

	}
}

//EllipseBy  椭圆
Point getEllipsePos(float a, float b, float angle, float t)
{
	float x = a*cos(2 * M_PI*t);
	float y = b*sin(2 * M_PI*t);

	float u = x*cos(angle) + y*sin(angle);
	float v = x*sin(angle) - y*cos(angle);

	return Vec2(u, v);
}


EllipseBy * EllipseBy::create(float t, Point& centerPosition, float aLength, float bLength, float angle)
{
	EllipseBy *pTuoyuanBy = new EllipseBy();
	pTuoyuanBy->initWithDuration(t, centerPosition, aLength, bLength, angle);
	pTuoyuanBy->autorelease();

	return pTuoyuanBy;
}




bool EllipseBy::initWithDuration(float t, Point& centerPosition, float aLength, float bLength, float angle)
{
	if (ActionInterval::initWithDuration(t))
	{
		_centerPosition = centerPosition;
		_aLength = aLength;
		_bLength = bLength;
		_angle = angle;
		return true;
	}

	return false;
}


void EllipseBy::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_previousPosition = target->getPosition();
}
void EllipseBy::update(float time)
{
	if (_target)
	{
		//rotate
		Point last = _previousPosition;
		Point current = _target->getPosition();
		_previousPosition = current;
		if (last.x != -500 || last.y != -500) {
			double angle = atan2((current.y - last.y), (current.x - last.x));
			_target->setRotation(90 - (angle * 360) / (2 * M_PI));
		}


		//
		Point s_startPosition = _centerPosition;//中心点坐标

		_target->setPosition(s_startPosition + getEllipsePos(_aLength, _bLength, _angle, time));//由于我们画计算出的椭圆你做值是以原点为中心的，所以需要加上我们设定的中心点坐标

	}
}



//HelixAction 螺旋
HelixAction* HelixAction::create(float t, Point center, float minRadius, float maxRadius, float rStartRadians)
{
	HelixAction *pc = new HelixAction();
	pc->initWithDuration(t, center, minRadius, maxRadius, rStartRadians);
	pc->autorelease();

	return pc;
}


bool HelixAction::initWithDuration(float t, Point center, float minRadius, float maxRadius, float rStartRadians)
{
	if (ActionInterval::initWithDuration(t))
	{
		_fCenter = center;
		_fMinRadius = minRadius;
		_fMaxRadius = maxRadius;
		_fStartRadians = rStartRadians;
		return true;
	}

	return false;
}

void HelixAction::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_previousPosition = _currentPosition = target->getPosition();
}



void HelixAction::update(float time)
{
	if (_target)
	{

		Point last = _previousPosition;
		Point current = _target->getPosition();
		_previousPosition = current;
		if (last.x != -500 || last.y != -500) {
			double angle = atan2((current.y - last.y), (current.x - last.x));
			_target->setRotation(90 - (angle * 360) / (2 * M_PI));
		}
		_target->setPosition(_fCenter + Vec2(((_fMaxRadius - _fMinRadius)*time + _fMinRadius)*cos(_fStartRadians + (6 * M_PI)*time), ((_fMaxRadius - _fMinRadius)*time + _fMinRadius)*sin(_fStartRadians + (6 * M_PI)*time)));
	}
}



// PathDraw 
std::unordered_map<Node*, int> PathDraw::_mGetIsContains;

PathDraw::~PathDraw()
{
}

PathDraw* PathDraw::create(Color4F color /*= Color4F(0, 0, 1, 1)*/)
{
	PathDraw *action = new (std::nothrow) PathDraw();
	if (action && action->initWithColor(color)){
		action->autorelease();
		return action;
	}
	return nullptr;
}

bool PathDraw::initWithColor(Color4F color)
{
	_color = color;
	return true;
}

void PathDraw::startWithTarget(cocos2d::Node *target)
{
	PathDraw* draw = static_cast<PathDraw*>(target->getActionByTag(ePathDrawTag));
	if (draw){
		draw->_color = this->_color;
		target->stopAction(this);
		return;
	}

	this->setTag(ePathDrawTag);
	ActionInterval::startWithTarget(target);

	auto root = target->getParent();
	if (_mGetIsContains.find(target) != _mGetIsContains.end()){
		root->stopAction(this);
		CCLOG("%s Had a PathDraw~~!!", target->getName().c_str());
	}
	else{
		_pDrawNode = DrawNode::create();
		_Monitoring = target;
		_index = _mGetIsContains.size();
		_mGetIsContains.emplace(_pDrawNode, _index);
		root->addChild(_pDrawNode);
		_pLastPos = target->getPosition();
	}

}

PathDraw* PathDraw::clone() const
{
	return PathDraw::create();
}

PathDraw* PathDraw::reverse(void) const
{
	return PathDraw::create();
}

void PathDraw::step(float dt)
{
	if (getTag() != ePathDrawTag){
		return;
	}
	auto node = _Monitoring;
	auto curPos = node->getPosition();
	if ((curPos - _pLastPos).length() < 10){
		_pDrawNode->drawSegment(_pLastPos, curPos, 2, _color);
	}
	_pLastPos = curPos;
}

bool PathDraw::isDone(void) const
{
	return false;
}

void PathDraw::stop()
{
	removeDrawNode();
	ActionInterval::stop();
}

void PathDraw::removeDrawNode()
{
	_mGetIsContains.erase(_Monitoring);
	_pDrawNode->removeFromParent();
}




//RotateCorrect 修正角度
RotateCorrect* RotateCorrect::create(float offsetAngle /*= 0*/)
{
	RotateCorrect *ref = new (std::nothrow) RotateCorrect();
	if (ref && ref->initWithOffsetAngle(offsetAngle))
	{
		ref->autorelease();
		return ref;
	}

	delete ref;
	return nullptr;
}

bool RotateCorrect::initWithOffsetAngle(float offsetAngle /*= 0*/)
{

	this->_offsetAngle = offsetAngle;

	return true;
}
void RotateCorrect::startWithTarget(Node *target)
{
	if (target->getActionByTag(eRotateCorrectTag)){
		target->stopAction(this);
		return;
	}
	this->setTag(eRotateCorrectTag);
	ActionInterval::startWithTarget(target);
	_startPosition = _currentPosition = _previousPosition = target->getPosition();
}

void RotateCorrect::step(float dt)
{
	if (getTag() != eRotateCorrectTag){
		return;
	}
	_currentPosition = _target->getPosition();
	if (_currentPosition != _previousPosition){
		double angle = atan2((_currentPosition.y - _previousPosition.y), (_currentPosition.x - _previousPosition.x));
		float newRotation = _offsetAngle - (angle * 360) / (2 * M_PI);
		_target->setRotation(newRotation);
	}
	_previousPosition = _currentPosition;
}

bool RotateCorrect::isDone(void) const
{
	return false;
}

RotateCorrect* RotateCorrect::clone() const
{
	return RotateCorrect::create(_offsetAngle);
}

RotateCorrect* RotateCorrect::reverse(void) const
{
	return RotateCorrect::create(_offsetAngle);
}


// RunAction
RunAction* RunAction::create(const RunningFunc& run)
{
	RunAction *ref = new (std::nothrow) RunAction();
	if (ref && ref->initWithFunc(run))
	{
		ref->autorelease();
		return ref;
	}

	delete ref;
	return nullptr;
}

RunAction* RunAction::clone() const
{
	return RunAction::create(_run);
}

RunAction* RunAction::reverse(void) const
{
	return RunAction::create(_run);
}

void RunAction::step(float dt)
{
	if (_run){
		_run(dt);
	}
}

bool RunAction::isDone(void) const
{
	return false;
}

bool RunAction::initWithFunc(const RunningFunc& run)
{
	_run = run;
	return true;
}