#ifndef _C2DCOMBASE_h_
#define _C2DCOMBASE_h_

#include "cocos2d.h"
#include "myObjectFactory.h"

USING_NS_CC;

#define CREATE_FUNC_PARAM_0(__TYPE__) \
	static __TYPE__* create() \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init()) \
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
#define CREATE_FUNC_PARAM_REF(__TYPE__) \
	static __TYPE__* create(cocos2d::Ref* var) \
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



#define OBJECT_DECLARE_CLASS_INFO_WITH_PARAM(__CLASS__) \
	CREATE_FUNC_PARAM_REF(__CLASS__); \
	public: \
	static cocos2d::c2d::myObjectFactory::TInfo _pType; \
	static cocos2d::Ref* createInstance(cocos2d::Ref* var); \


#define OBJECT_IMPLEMENT_CLASS_INFO_WITH_PARAM(name,__CLASS__) \
	cocos2d::Ref* __CLASS__::createInstance(cocos2d::Ref* var) \
{ \
	return __CLASS__::create(var); \
} \
	cocos2d::c2d::myObjectFactory::TInfo __CLASS__::_pType(name, &__CLASS__::createInstance); \

#define OBJECT_DECLARE_CLASS_INFO_NO_PARAM(__CLASS__) \
	CREATE_FUNC_PARAM_0(__CLASS__); \
	public: \
	static cocos2d::c2d::myObjectFactory::TInfo _pType; \
	static cocos2d::Ref* createInstance(); \


#define OBJECT_IMPLEMENT_CLASS_INFO_NO_PARAM(name,__CLASS__) \
	cocos2d::Ref* __CLASS__::createInstance(void) \
{ \
	return __CLASS__::create(); \
} \
	cocos2d::c2d::myObjectFactory::TInfo __CLASS__::_pType(name, &__CLASS__::createInstance); \

#define CREATE_OBJECT_FROM_FACTORY_PARAM_1(__KEY__,_VAR__)\
	cocos2d::c2d::myObjectFactory::getInstance()->createObject(__KEY__, _VAR__)

#define CREATE_OBJECT_FROM_FACTORY_PARAM_0(__KEY__)\
	cocos2d::c2d::myObjectFactory::getInstance()->createObject(__KEY__)

#define CREATE_NODE_FROM_FACTORY_PARAM_1(__KEY__,_VAR__)\
	static_cast<Node*>(cocos2d::c2d::myObjectFactory::getInstance()->createObject(__KEY__, _VAR__))

#define CREATE_NODE_FROM_FACTORY_PARAM_0(__KEY__)\
	static_cast<Node*>(cocos2d::c2d::myObjectFactory::getInstance()->createObject(__KEY__))


#define CREATE_CLASS_COMPONENT_INFO_C2D(__CLASS__) \
	cocos2d::c2d::myObjectFactory::TInfo(#__CLASS__, &__CLASS__::createInstance)



#endif
