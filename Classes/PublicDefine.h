#ifndef _publicDefine_h_
#define _publicDefine_h_


#include "PublicCoco.h"
  
typedef std::function<void()> Runnable;


#define CREATE_FUNC_PARAM_1(__TYPE__,__CLASS__) \
	static __TYPE__* create(__CLASS__ var) \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init(var)) \
{ \
	pRet->autorelease(); \
	return pRet; \
} \
	else \
{ \
	delete pRet; \
	pRet = nullptr; \
	return nullptr; \
} \
}


#define CREATE_FUNC_PARAM_2(__TYPE__,__CLASS_0__,__CLASS_1__) \
	static __TYPE__* create(__CLASS_0__ var0, __CLASS_1__ var1) \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init(var0, var1)) \
{ \
	pRet->autorelease(); \
	return pRet; \
} \
	else \
{ \
	delete pRet; \
	pRet = nullptr; \
	return nullptr; \
} \
}

#define CREATE_INSTANCE_FUNC(__CLASS__) \
private:\
    static __CLASS__ *_gInstance;\
public:\
static __CLASS__* getInstance()\
{\
    if(!_gInstance)\
    {\
        _gInstance = new __CLASS__();\
        if(_gInstance && _gInstance->init())\
        {\
        }\
        else\
        {\
            CC_SAFE_DELETE(_gInstance);\
        }\
    }\
    return _gInstance;\
}

#define DESTROY_INSTANCE_FUNC()\
static void destroyInstance()\
{\
    CC_SAFE_RELEASE_NULL(_gInstance);\
}

#define CAST_DYNAMIC_FUNC(__CLASS__)\
	static __CLASS__* to##__CLASS__(Ref* var)\
{\
	return dynamic_cast<__CLASS__*>(var); \
}
// SpineFace* Fish::getSpineFace()
// {
// 	return dynamic_cast<SpineFace*>(_pSkin);
// }



//添加 Touch 监听
#define LISTENER_TOUCH_ONE_BY_BOE(__TYPE__,needSwallow) \
	auto listenerTouchOneByOne = EventListenerTouchOneByOne::create(); \
	listenerTouchOneByOne->setSwallowTouches(needSwallow); \
	listenerTouchOneByOne->onTouchBegan = CC_CALLBACK_2(__TYPE__::onTouchBegan, this); \
	listenerTouchOneByOne->onTouchMoved = CC_CALLBACK_2(__TYPE__::onTouchMoved, this); \
	listenerTouchOneByOne->onTouchEnded = CC_CALLBACK_2(__TYPE__::onTouchEnded, this); \
	listenerTouchOneByOne->onTouchCancelled = CC_CALLBACK_2(__TYPE__::onTouchCancelled, this); \
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouchOneByOne, this);


//std::vector 移除元素
#define vectorEraseObject(vec,beRemoveEle)\
{\
	auto it = find(vec.begin(), vec.end(), beRemoveEle); \
	if (it != vec.end()){\
		vec.erase(it);\
	}else{\
	}\
}
 


/// @name Cocos2d debug
/// @{
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
#define DELOG(file,line,s, ...)       do {} while (0)

#elif COCOS2D_DEBUG == 1
#define DELOG(file,line,s, ...) {\
	std::string info = StringUtils::format("%s,line:%d", StringUtil::getFileName(file).c_str(), line); \
	cocos2d::log("%s -> %s", info.c_str(), cocos2d::StringUtils::format(s, ##__VA_ARGS__).c_str()); \
}


#elif COCOS2D_DEBUG > 1
#define DELOG(file,line,s, ...) {\
	std::string info = StringUtils::format("%s,line:%d", StringUtil::getFileName(file).c_str(), line); \
	cocos2d::log("%s -> %s", info.c_str(), cocos2d::StringUtils::format(s, ##__VA_ARGS__).c_str()); \
}

#endif // COCOS2D_DEBUG


#define winSize Director::getInstance()->getWinSize()//设计分辨率
#define screenSize Director::getInstance()->getVisibleSize()//显示分辨率
#define machineSize Director::getInstance()->getOpenGLView()->getFrameSize()//设备分辨率
#define designSize Size(1536, 768)
#define screenPos Vec2(Vec2(designSize - screenSize)/2)


#define Notify cocos2d::NotificationCenter::getInstance()

#define easyToString(val) (#val)



#endif
