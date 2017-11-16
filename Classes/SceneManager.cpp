#include "SceneManager.h"
//#include "Test/SceneLoading.h"

SceneManager *SceneManager::_gInstance;


#define CREATE_SCENE(name) (static_cast<Scene*>(cocos2d::ObjectFactory::getInstance()->createObject(name)))


SceneManager::STypeInfo::STypeInfo(SceneType key, const std::string& value)
{
	SceneManager::getInstance()->addScenePair(key, value);
}


bool SceneManager::init()
{
    bool bRet = false;
    do
    {
        bRet = true;
    }
    while (0);
    return bRet;
}

void SceneManager::addScenePair(SceneType key, const std::string& value)
{
	_pSceneTypeStrMap.emplace(key, value);
}

void SceneManager::initLoading()
{
	//changeScene(eSceneMain);
	//replaceScene(eSceneNone);
	//replaceScene(eScene3DTest);
	replaceScene(eSceneTennisGame);
	//replaceSceneWithLoading(eSceneMain);
	//replaceSceneWithLoading(eSceneMain);
}

Scene* SceneManager::creatScene(SceneType rEnSceneType)
{
	Scene* scene = nullptr;
	
	std::string sceneStr = "";
	if (_pSceneTypeStrMap.find(rEnSceneType) != _pSceneTypeStrMap.end()){
		sceneStr = _pSceneTypeStrMap.at(rEnSceneType);
		scene = CREATE_SCENE(sceneStr);
	}
	else{
		CCLOG("have no the scene -> %d", rEnSceneType);
	}
	
	scene->setUserData(&rEnSceneType);
	return scene;
}

void SceneManager::replaceSceneWithLoading(SceneType rEnSceneType)
{
// 	Scene* scene = SceneLoading::createScene(rEnSceneType);
// 
// 	auto loadtype = eSceneLoading;
// 	scene->setUserData(&loadtype);
// 
// 	replaceScene(loadtype, scene);
}

void SceneManager::replaceScene(const std::string& rEnSceneTypeStr, Scene* scene /*= nullptr*/)
{
	SceneType rEnSceneType = eSceneNone;
	if (scene == nullptr){
		scene = CREATE_SCENE(rEnSceneTypeStr);
		for (auto item : _pSceneTypeStrMap)
		{
			if (item.second == rEnSceneTypeStr){
				rEnSceneType = item.first;
			}
		}
		scene->setUserData(&rEnSceneType);
	}
	auto pDirector = Director::getInstance();
	auto pRunScene = pDirector->getRunningScene();
	cacheScene(rEnSceneType);

	if (pRunScene == NULL){
		pDirector->runWithScene(scene);
	}
	else{
		auto pTransScene = TransitionFade::create(0.5f, scene, Color3B(255, 255, 255));
		pDirector->replaceScene(pTransScene);
	}
}

void SceneManager::replaceScene(SceneType rEnSceneType,Scene* scene /*= nullptr*/)
{
	if (scene == nullptr){
		scene = creatScene(rEnSceneType);
	}
	auto pDirector = Director::getInstance();
	auto pRunScene = pDirector->getRunningScene();
	cacheScene(rEnSceneType);
	if (pRunScene == NULL){
		pDirector->runWithScene(scene);
	}
	else{
		auto pTransScene = TransitionFade::create(0.5f, scene, Color3B(255, 255, 255));
		pDirector->replaceScene(pTransScene);
	}
}

void SceneManager::replaceSceneCustomTransition(SceneType rEnSceneType, std::function<TransitionScene*(Scene*)> rTransitionSceneCallback /*= nullptr*/)
{

	auto scene = creatScene(rEnSceneType);
	
	auto pDirector = Director::getInstance();
	auto pRunScene = pDirector->getRunningScene();
	cacheScene(rEnSceneType);
	if (pRunScene == NULL){
		pDirector->runWithScene(scene);
	}
	else{
	
		if (rTransitionSceneCallback){
			TransitionScene* pTransScene  = rTransitionSceneCallback(scene);

			pDirector->replaceScene(pTransScene);
		}
		else{	
			pDirector->replaceScene(scene);
		}
	}
}
void SceneManager::replaceSceneWithLastScene()
{
	replaceScene(_enLastScene);
}


void SceneManager::pushScene(SceneType rEnSceneType, Scene* scene /*= nullptr*/)
{
	if (scene == nullptr){
		scene = creatScene(rEnSceneType);
	}
	auto pDirector = Director::getInstance();
	auto pRunScene = pDirector->getRunningScene();
	
	cacheScene(rEnSceneType);
	if (pRunScene == NULL){
		pDirector->runWithScene(scene);
	}
	else{
		auto pTransScene = TransitionFade::create(0.5f, scene, Color3B(255, 255, 255));
		pDirector->pushScene(pTransScene);
	}
}

void SceneManager::popScene()
{
	//replaceScene(_enLastScene);
	auto pDirector = Director::getInstance();
	pDirector->popScene();
	pDirector->drawScene();
	auto scene = pDirector->getRunningScene();

	auto curscene = *static_cast<SceneType*>(scene->getUserData());

	cacheScene(curscene);

// 	SceneType temp = _enCurScene;
// 	_enCurScene = _enLastScene;
// 	_enLastScene = temp;
}

void SceneManager::cacheScene(SceneType rEnSceneType)
{
	if (_enLastScene != _enCurScene){
		_enLastScene = _enCurScene;
	}
	_enCurScene = rEnSceneType;
}

bool SceneManager::isScene(SceneType rEnSceneType)
{
	return _enCurScene == rEnSceneType;
}

void SceneManager::onExit()
{
	Node::onExit();
}

void SceneManager::onEnter()
{
	Node::onEnter();
}

