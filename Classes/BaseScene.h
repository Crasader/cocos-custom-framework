#ifndef __BaseScene_H__
#define __BaseScene_H__

#include "PublicDefine.h"
#include "Align.h"
#include "Finder.h"
#include "SceneManager.h"

#define SCENE_REGISTER_INFO \
	public: \
	static Scene* createScene();\
	static cocos2d::ObjectFactory::TInfo _pType; \
	static SceneManager::STypeInfo _STypeInfo; \
	static cocos2d::Ref* createInstance(void); \

#define SCENE_IMPLEMENT_INFO(sceneType,className) \
	Scene* className::createScene()\
	{\
		auto scene = Scene::create();\
		auto layer = className::create(); \
		scene->addChild(layer);\
		return scene;\
	}\
	cocos2d::Ref* className::createInstance(void) \
		{ \
		return className::createScene(); \
		} \
		cocos2d::ObjectFactory::TInfo className::_pType(#sceneType, &className::createInstance); \
		SceneManager::STypeInfo className::_STypeInfo(sceneType, #sceneType); \

#define CREATE_SCENE_INFO(sceneType,className) \
	cocos2d::ObjectFactory::TInfo(#sceneType, &className::createInstance)


class BaseScene : public cocos2d::Layer,public FinderAdapter
{
public:

	virtual bool initBaseScene();
	virtual bool init(const std::string& str);

	virtual void adaptScreen(const std::string& path,c2d::Align align = c2d::Align::center);
	virtual void adaptScreenChildren(const std::string& path, c2d::Align align = c2d::Align::center);

	virtual Button* setBtnClickListener(const std::string& path, const std::function<void(Ref*)>& _clickCallBack);

	virtual Node* getChildByPath(const std::string& path);
	template<typename T> inline
		T getChildByPath(const std::string& path)
	{
			return dynamic_cast<T>(getChildByPath(path));
	}


 	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
 	virtual void onKeyBack();
 	virtual void onKeyMenu();


protected:
	virtual void registerEvent();

protected:
	Node* _pRoot = nullptr;
	Button* btnBack = nullptr;
};

#endif /* defined(__BaseScene_H__) */
