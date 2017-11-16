# ifndef __SceneType_H__
# define __SceneType_H__

#include "cocos2d.h"
NS_CC_BEGIN
//直接在最后追加，不要打乱顺序
enum SceneType
{
	eSceneNone = -2,
	eSceneTest = -1,
	eSceneLoading,
	eSceneMain,
	eSceneDressing,
	eSceneGame,//作为游戏 ui
	eScenePractice,
	eSceneKingLines,
	eSceneTennisGame,//网球游戏界面
	eScene3DTest,//3d 测试
};

NS_CC_END
#endif /* defined(__SceneType_H__) */
