#ifndef __PathDrawAgent_h__  
#define __PathDrawAgent_h__  

#include "cocos2d.h"    
USING_NS_CC;

class PathDrawAgent : public Node
{

public:
	static void registered(Node* target, Color4F color = Color4F(0, 1, 0, 1));
	static void unregistered(Node* target);
	static void changeColor(Node* target,Color4F color = Color4F(0, 1, 0, 1));
	virtual bool init();

	CREATE_FUNC(PathDrawAgent);

	virtual void update(float delta);

	virtual void setDrawNodeColor(Color4F color);

protected:
	virtual void setTarget(Node* target);

private:
	static std::unordered_map<Node*, PathDrawAgent*> _mGetIsContains1;
	Vec2 _pLastPos;
	DrawNode* _pDrawNode = nullptr;
	Node* _target = nullptr;
	Color4F _color;
	int _index;
};


#endif // __PathDrawAgent_h__  