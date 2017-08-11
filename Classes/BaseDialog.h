#ifndef __BaseDialog_H__
#define __BaseDialog_H__

#include "BasePage.h"


#define DIALOG_REGISTER_INFO \
	public: \
	static cocos2d::ObjectFactory::TInfo _pTInfo; \
	static cocos2d::Ref* createInstance(void); \

#define DIALOG_IMPLEMENT_INFO(name,className) \
	cocos2d::Ref* className::createInstance(void) \
		{ \
		return className::create(); \
		} \
		cocos2d::ObjectFactory::TInfo className::_pTInfo(name, &className::createInstance); \

#define CREATE_DIALOG_INFO(name,className) \
	cocos2d::ObjectFactory::TInfo(name, &className::createInstance)

#define DIALOG_INIT_DEFAULT_FUNC \
private:\
	virtual bool init() final { return BaseDialog::init(_pTInfo._class); }\

#define DIALOG_CREATE_FUNC_PARAM_1(__TYPE__)\
public:\
	static __TYPE__* create(Ref* var) \
	{ \
		__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
		if (pRet && pRet->init() && pRet->initWithParam(var)) \
		{ \
			pRet->autorelease(); \
			return pRet; \
		}else{ \
			delete pRet; \
			pRet = nullptr; \
			return nullptr; \
		} \
	}\
public:\
	static __TYPE__* create() \
	{ \
		__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
		if (pRet && pRet->init()) \
			{ \
				pRet->autorelease(); \
				return pRet; \
			}else{ \
				delete pRet; \
				pRet = nullptr; \
				return nullptr; \
			} \
	}

#define DIALOG_CREATE_FUNC_PARAM_0(__TYPE__)\
public:\
	   static __TYPE__* create() \
	{ \
		__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
		if (pRet && pRet->init() && pRet->initWithNoParam()) \
		{ \
			pRet->autorelease(); \
			return pRet; \
		}else{\
			delete pRet; \
			pRet = nullptr; \
			return nullptr; \
		} \
	}

#define MAKE_TUPLE_TO_REF(...)\
	(reinterpret_cast<Ref*>(&std::make_tuple(##__VA_ARGS__)))
#define GET_TUPLE_FROM_REF(ref,...)\
	(*(reinterpret_cast<std::tuple<##__VA_ARGS__>*>(ref)))


class BasePage;
class BaseDialog :public BasePage
{
public:
public:
	BaseDialog();
	~BaseDialog();

	static BaseDialog* create(const std::string& filename);
	static BaseDialog* create(Node* node);
	
	virtual bool initWithParam(Ref* param);//有参数的时候需要实现的初始化函数
	virtual bool initWithNoParam();//没有参数的时候需要实现的初始化函数

	virtual void show();
	virtual void close() final;

	virtual float doShowAction();
	virtual float doCloseAction();

	virtual float getShowDuration();
	virtual float getCloseDuration();

	virtual void onShowCallback();
	virtual void onCloseCallback();

	virtual void setShowCallbackFunc(std::function<void()> showCallbackFunc);
	virtual void setCloseCallbackFunc(std::function<void()> closeCallbackFunc);

	virtual float behindScene();//暂退
	virtual float recoverScene();//恢复

	virtual void onBehindSceneCallback();
	virtual void onRecoverSceneCallback();
	
	virtual void setBehindSceneCallback(std::function<void()> behindSceneCallbackFunc);
	virtual void setRecoverSceneCallback(std::function<void()> recoverSceneCallbackFunc);

	virtual void setAutoCloseDelay(float delay);
	virtual float getAutoCloseDelay();
	virtual BaseDialog* setAutoClose(bool bAutoClose);
	virtual bool isAutoClose();
	
	virtual BaseDialog* setSingleton(bool bSingleton);
	virtual bool isSingleton();

	virtual BaseDialog* setListenBackKey(bool bListenBackKey);
	virtual bool isListenBackKey();

	virtual BaseDialog* setTapBackGroundClose(bool bTapBackGroundClose);
	virtual bool isTapBackGroundClose();

	virtual void setPreviousDialog(BaseDialog* rPreviousDialog);
	virtual BaseDialog* getPreviousDialog();

	virtual BaseDialog* setMaskColor(const Color4F& color = Color4F(0, 0, 0, 0.6f));
	virtual Button* setBtnClickListener(const std::string& path, const ccWidgetClickCallback& _clickCallBack);
	virtual Button* setCloseBtn(const std::string& path);

	virtual std::vector<Node*> getNodeVector(const char* name,...);
	virtual void showButtonDelayPop(const char* name, ...);

	virtual void showButtonDelayPop(Node* node, ...);//最后一个参数必须为 nullptr
	virtual void showButtonDelayPop(std::vector<Node*> btns);
	virtual void showButtonDelayPop(std::vector<Node*> btns, float delay, float duration);
	virtual bool onKeyBack();

protected:
	virtual std::vector<Node*> getNodeVectorFrom_va_list(const char* name, va_list args);

protected:
	virtual void initAdapter();

	float _fDelayAutoClose;
	bool _bIsSingleton;//是否唯一
	bool _bIsAutoClose;//是否自动关闭 -- 需要在
	
	bool _bIsListenBackKey;//是否监听返回键
	bool _bIsTapBackGroundClose; // 是否点击背景关闭
	
	BaseDialog* _pPreviousDialog;//被压入的界面

	std::function<void()> _showCallbackFunc = nullptr;
	std::function<void()> _closeCallbackFunc = nullptr;

	std::function<void()> _behindSceneCallbackFunc = nullptr;
	std::function<void()> _recoverSceneCallbackFunc = nullptr;
};


class DialogHelper
{
public:
	DialogHelper();
	~DialogHelper();
	static BaseDialog* createUIWithParam(const std::string& filepath, Ref* param);
	static BaseDialog* ShowUI(const std::string& filepath, bool push = true);
	static BaseDialog* ShowUI(const std::string& filepath,Ref* param, bool push = true);
	static BaseDialog* CloseUI(const std::string& filepath);
	static BaseDialog* CloseUI(BaseDialog* dialog);
	static BaseDialog* CloseUI();
	static bool onKeyBack();
private:

};



#endif /* defined(__BaseDialog_H__) */
