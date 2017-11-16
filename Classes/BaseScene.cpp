#include "BaseScene.h"
#include "UiManager.h"
#include "c2d.h"
bool BaseScene::init(const std::string& str)
{
	_pRoot = CSLoader::createNode(str);
	this->addChild(_pRoot);

	return this->initBaseScene();
}

bool BaseScene::initBaseScene()
{
    if ( !FinderAdapter::init(this)||!Layer::init())
    {
        return false;
    }

	////按键控制（检测onKeyReleased有反应）
	//auto listenerKeyboard = EventListenerKeyboard::create();
	//listenerKeyboard->onKeyReleased = CC_CALLBACK_2(BaseScene::onKeyReleased, this);
	//Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerKeyboard, this);
	setKeyboardEnabled(true);
	registerEvent();

	if (_pRoot == nullptr){
		_pRoot = this;
	}
	this->setName("Root");
    return true;
}

void BaseScene::adaptScreen(const std::string& path, c2d::Align align /*= c2d::Align::center*/)
{
	c2d::adaptScreen(_pRoot, path, align);
}

void BaseScene::adaptScreenChildren(const std::string& path, c2d::Align align /*= c2d::Align::center*/)
{
	c2d::adaptScreenChildren(_pRoot, path, align);
}

Node* BaseScene::getChildByPath(const std::string& path)
{
	auto node = Finder::getChildByPath(this, path);
	if (node == nullptr){
		node = Finder::getChildByPath(_pRoot, path);
	}
	return node;
}

Button* BaseScene::setBtnClickListener(const std::string& path, const std::function<void(Ref*)>& _clickCallBack)
{
	auto btn = getChildByPath<Button*>(path);
	btn->addClickEventListener(_clickCallBack);
	return btn;
}
 
 void BaseScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
 {
	 Layer::onKeyReleased(keyCode, event);
 	switch (keyCode)
 	{
 		//监听返回键
 	case EventKeyboard::KeyCode::KEY_ESCAPE:
 		onKeyBack();
 		break;
 		//监听menu键
 	case EventKeyboard::KeyCode::KEY_MENU:
 		onKeyMenu();
 		break;
 	}
 }

 void BaseScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
 {
	 Layer::onKeyPressed(keyCode, event);
 }

 void BaseScene::onKeyBack()
 {
	 if (UiManager::onKeyBack() == false){
		 //SDKManager::mainsceneexit();
	 }
 }
 
 void BaseScene::onKeyMenu()
 {
	
 }

 void BaseScene::registerEvent()
 {

 }
