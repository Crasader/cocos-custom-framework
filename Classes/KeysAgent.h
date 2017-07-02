#ifndef __KeysAgent_h__  
#define __KeysAgent_h__  

#include "cocos2d.h"    
USING_NS_CC;

class KeysAgent : public cocos2d::Layer
{
public: 	 
	static void registered(Node* node, std::function<void()> _doKey, EventKeyboard::KeyCode rKeyCode = EventKeyboard::KeyCode::KEY_F2);

	virtual bool init();
	 
	CREATE_FUNC(KeysAgent);

	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

	virtual void screenshot();
	
protected:
	
	EventKeyboard::KeyCode _pKeyCode;
	std::function<void()> _doKey;
protected:
};

#endif // __KeysAgent_h__  