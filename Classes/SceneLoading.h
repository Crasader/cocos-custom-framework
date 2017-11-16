#ifndef __SceneLoading_H__
#define __SceneLoading_H__

#include "BaseScene.h"
#include "BaseLoading.h"
#include "PublicDefine.h"

class SceneLoading : public BaseScene, public BaseLoading
{
public:
	static Scene* createScene(SceneType nextScene);
	CREATE_FUNC_PARAM_1(SceneLoading, SceneType);
	virtual bool init(SceneType nextScene);
	
	virtual void onLoadEnd();
	virtual void onLoadJsonDataCallBack(const std::string& filename,int index);
	virtual void onLoadJsonDataEndCallBack();
protected:
	virtual void onInit();
	virtual void update(float delta);

	LoadingBar* _pLoadingBar;
	SceneType _pNextScene;
};

#endif /* defined(__SceneLoading_H__) */
