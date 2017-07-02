#include "PathDrawAgent.h"  


std::unordered_map<Node*, PathDrawAgent*> PathDrawAgent::_mGetIsContains1;

void PathDrawAgent::registered(Node* target, Color4F color /*= Color4F(0, 0, 1, 1)*/)
{
	if (_mGetIsContains1.find(target) != _mGetIsContains1.end()){
		auto _PathDrawAgent = _mGetIsContains1.at(target);
		_mGetIsContains1.erase(target);

		_PathDrawAgent->removeFromParent();
	}
	auto _PathDrawAgent = PathDrawAgent::create();
	_PathDrawAgent->setTarget(target);
	_PathDrawAgent->setDrawNodeColor(color);

	_mGetIsContains1.emplace(target, _PathDrawAgent);
}

void PathDrawAgent::unregistered(Node* target)
{
	if (_mGetIsContains1.find(target) != _mGetIsContains1.end()){
		auto _PathDrawAgent = _mGetIsContains1.at(target);
		_mGetIsContains1.erase(target);

		_PathDrawAgent->removeFromParent();
	}
	else{

	}
}

void PathDrawAgent::changeColor(Node* target, Color4F color /*= Color4F(0, 1, 0, 1)*/)
{
	if (_mGetIsContains1.find(target) != _mGetIsContains1.end()){
		auto _PathDrawAgent = _mGetIsContains1.at(target);
		_PathDrawAgent->setDrawNodeColor(color);
	}
}
bool PathDrawAgent::init()
{
	scheduleUpdate();

	return true;
}

void PathDrawAgent::setTarget(Node* target)
{
	if (_target == nullptr){
		auto root = target->getParent();
		root->addChild(this);
		_target = target;
		_pDrawNode = DrawNode::create();
		this->addChild(_pDrawNode);
		_pLastPos = target->getPosition();
	}
}

void PathDrawAgent::setDrawNodeColor(Color4F color)
{
	_color = color;
}

void PathDrawAgent::update(float delta)
{
	auto node = _target;
	auto curPos = node->getPosition();
	if ((curPos - _pLastPos).length() < 25 && _pLastPos != curPos){
		_pDrawNode->drawSegment(_pLastPos, curPos, 2, _color);
	}
	_pLastPos = curPos;
}

