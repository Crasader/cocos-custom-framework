#include "KeysAgent.h"  
#include "PublicDefine.h"
USING_NS_CC;
static int _count = 0;
//static std::unordered_map<EventKeyboard::KeyCode, Node*> hashmap;
void KeysAgent::registered(Node* node, std::function<void()> rDoKey, EventKeyboard::KeyCode rKeyCode)
{
// 	if (hashmap.find(rKeyCode) != hashmap.end()){
// 		auto node = hashmap.at(rKeyCode);
// 		auto str = "the node [" + node->getName() + "] is used this key ->"+StringUtil::Int2String((int)rKeyCode);
// 		UiUtils::ShowWarning(str.c_str());
// 		return;
// 	}
	
	auto _KeysAgent = KeysAgent::create();

	_KeysAgent->_pKeyCode = rKeyCode;
	_KeysAgent->_doKey = rDoKey;

	node->addChild(_KeysAgent);
	//hashmap[rKeyCode] = node;
}

bool KeysAgent::init()
{

	if (!Layer::init())
	{
		return false;
	}

	//×¢²á²¶×½¼àÌý
	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(KeysAgent::onKeyReleased, this);
	listenerkeyPad->onKeyPressed = CC_CALLBACK_2(KeysAgent::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
	return true;
}

void KeysAgent::onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == _pKeyCode){
		if (_doKey){
			_doKey();
		}
		//CCLOG("press key -> %d", _pKeyCode);
	}

	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_NONE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_PAUSE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SCROLL_LOCK:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_PRINT:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SYSREQ:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_BREAK:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_BACKSPACE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_TAB:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_BACK_TAB:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RETURN:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPS_LOCK:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SHIFT:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_SHIFT:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CTRL:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_ALT:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ALT:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_MENU:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_HYPER:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_INSERT:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_HOME:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_PG_UP:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DELETE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_END:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_PG_DOWN:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_NUM_LOCK:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_PLUS:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_MINUS:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_MULTIPLY:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_DIVIDE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_HOME:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_UP:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_PG_UP:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_LEFT:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_FIVE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_RIGHT:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_END:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_DOWN:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_PG_DOWN:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_INSERT:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_DELETE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F1:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F2:
		screenshot();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F3:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F4:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F5:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F6:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F7:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F8:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F9:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F10:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F11:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F12:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_EXCLAM:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_QUOTE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_NUMBER:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOLLAR:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_PERCENT:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CIRCUMFLEX:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_AMPERSAND:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_APOSTROPHE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_PARENTHESIS:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_PARENTHESIS:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_ASTERISK:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_PLUS:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_COMMA:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_MINUS:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_PERIOD:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SLASH:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_0:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_1:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_2:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_3:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_4:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_5:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_6:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_7:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_8:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_9:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_COLON:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SEMICOLON:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LESS_THAN:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_EQUAL:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_GREATER_THAN:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_QUESTION:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_AT:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_B:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_C:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_E:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_F:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_G:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_H:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_I:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_J:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_K:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_L:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_M:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_N:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_O:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_P:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_Q:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_R:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_T:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_U:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_V:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_X:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_Y:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_Z:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_BRACKET:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_BACK_SLASH:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_BRACKET:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UNDERSCORE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_GRAVE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_B:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_C:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_E:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_G:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_H:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_I:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_L:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_M:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_N:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_O:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_P:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_Q:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_R:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_T:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_U:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_V:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_X:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_Y:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_Z:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_BRACE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_BAR:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_BRACE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_TILDE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_EURO:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_POUND:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_YEN:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_MIDDLE_DOT:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SEARCH:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_LEFT:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_RIGHT:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_UP:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_DOWN:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_CENTER:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_ENTER:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_PLAY:
		break;
	default:
		break;
	}
}

void KeysAgent::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{

}

void KeysAgent::screenshot()
{
	utils::captureScreen([=](bool is,std::string fullpath){
		CCLOG("screenshot_paht -> %s", fullpath.c_str());
	}, StringUtils::format("screenshot_%d.png", _count));
	_count++;
}
