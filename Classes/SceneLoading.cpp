#include "SceneLoading.h"
#include "SceneManager.h"
#include "PublicDefine.h"
#include "DataManager.h"
Scene* SceneLoading::createScene(SceneType nextScene)
{
	auto scene = Scene::create();
	auto layer = SceneLoading::create(nextScene);
	scene->addChild(layer);
	return scene;
}

bool SceneLoading::init(SceneType nextScene)
{
// 	if (BaseScene::init(csb_SceneLoading)){
// 	
// 		_pNextScene = nextScene;
// 		GlobalSchedule::getInstance();
// 		onInit();
// 		return true;
// 	}
	return false;
}

void SceneLoading::onInit()
{
	scheduleUpdate();

	// init UI
// 	_pLoadingBar = static_cast<LoadingBar*> (getChildByPath("LoadingBg_Image/LoadingBar"));
// 	_pLoadingBar->setPercent(0);
// 	auto actio = CSLoader::createTimeline(csb_SceneLoading);
// 	actio->play("loading", true);
// 	_pRoot->runAction(actio);
// 
// 
// 	//init data
// 	switch (_pNextScene)
// 	{
// 	case cocos2d::eSceneMain:
// 		addCsb(csb_SceneMain);
// 		
// 		addPlist(plist_PlistDressRoomShoe);
// 
// 		addJsonData(json_Config);
// 		addJsonData(json_Language);
// 		break;
// 	case cocos2d::eSceneGame:
// 		
// 		break;
// 	default:
// 		break;
// 	}

	beginLoad();

}

void SceneLoading::update(float delta)
{
	_pLoadingBar->setPercent(getVisualPercent(getPercent()));
}

void SceneLoading::onLoadEnd()
{
	BaseLoading::onLoadEnd();
	SceneManager::getInstance()->replaceScene(_pNextScene);
}

void SceneLoading::onLoadJsonDataCallBack(const std::string& filename,int index)
{
	

}

void SceneLoading::onLoadJsonDataEndCallBack()
{
	
}

