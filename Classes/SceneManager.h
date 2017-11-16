#ifndef __SceneManager__
#define __SceneManager__

#include "PublicDefine.h"
#include "SceneType.h"


class SceneManager:public Node
{
public:
	struct STypeInfo
	{
		STypeInfo(SceneType key, const std::string& value);
	};
public:
    
    CREATE_INSTANCE_FUNC(SceneManager);
    
    DESTROY_INSTANCE_FUNC();
    
	virtual void initLoading();
	virtual Scene* creatScene(SceneType rEnSceneType);
	virtual void replaceScene(const std::string& rEnSceneType, Scene* rEnScene = nullptr);
	virtual void replaceScene(SceneType rEnSceneType,Scene* rEnScene = nullptr);
	virtual void replaceSceneCustomTransition(SceneType rEnSceneType, std::function<TransitionScene*(Scene*)> rTransitionSceneCallback = nullptr);
	virtual void replaceSceneWithLoading(SceneType rEnSceneType);
	virtual void replaceSceneWithLastScene();
	virtual void cacheScene(SceneType rEnSceneType);

	virtual void pushScene(SceneType rEnSceneType, Scene* rEnScene = nullptr);
	virtual void popScene();
	virtual bool isScene(SceneType rEnSceneType);

	virtual void onExit();
	virtual void onEnter();

	virtual void addScenePair(SceneType key,const std::string& value);

	virtual SceneType& getLastScene(){ return _enLastScene; };
	virtual SceneType& getCurScene(){ return _enCurScene; };
protected:
    
	
    virtual bool init();
    
protected:
    
	SceneType _enCurScene = eSceneNone;
	SceneType _enLastScene = eSceneNone;

	std::map<SceneType, std::string> _pSceneTypeStrMap;
private:
    
private:
    
//     SceneManager(){}
//     
//     virtual ~SceneManager(){}
//     
//     SceneManager(const SceneManager &) = delete;
//     
//     SceneManager &operator=(const SceneManager &) = delete;
    
};

#endif /* defined(__SceneManager__) */
