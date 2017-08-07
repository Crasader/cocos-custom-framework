#ifndef __DIALOGADAPTER_H__
#define __DIALOGADAPTER_H__

#include "PageAdapter.h"




#define DIALOG_REGISTER_INFO \
	public: \
	static cocos2d::ObjectFactory::TInfo _pType; \
	static cocos2d::Ref* createInstance(void); \

#define DIALOG_IMPLEMENT_INFO(name,className) \
	cocos2d::Ref* className::createInstance(void) \
		{ \
		return className::create(); \
		} \
		cocos2d::ObjectFactory::TInfo className::_pType(name, &className::createInstance); \

#define CREATE_DIALOG_INFO(name,className) \
	cocos2d::ObjectFactory::TInfo(name, &className::createInstance)



class PageAdapter;
class DialogAdapter :public PageAdapter
{
public:
public:
	DialogAdapter();
	~DialogAdapter();

	static DialogAdapter* create(const std::string& filename);
	static DialogAdapter* create(Node* node);
	
	virtual void show();
	virtual void close() final;

	virtual float doShowAction();
	virtual float doCloseAction();

	virtual float getShowDuration();
	virtual float getCloseDuration();

	virtual void onShowCallback();
	virtual void onCloseCallback();

	virtual void registerEventDispatcher();

	virtual float behindScene();//����
	virtual float recoverScene();//�ָ�

	virtual void onBehindSceneCallback();
	virtual void onRecoverSceneCallback();

	virtual DialogAdapter* setAutoClose(bool bAutoClose);
	virtual bool isAutoClose();
	
	virtual DialogAdapter* setSingleton(bool bSingleton);
	virtual bool isSingleton();

	virtual DialogAdapter* setListenBackKey(bool bListenBackKey);
	virtual bool isListenBackKey();

	virtual DialogAdapter* setTapBackGroundClose(bool bTapBackGroundClose);
	virtual bool isTapBackGroundClose();

	virtual void setPreviousDialog(DialogAdapter* rPreviousDialog);
	virtual DialogAdapter* getPreviousDialog();

	virtual DialogAdapter* setMaskColor(const Color4F& color = Color4F(0, 0, 0, 0.6f));
	virtual Button* setBtnClickCallback(const std::string& path, const ccWidgetClickCallback& _clickCallBack);
	virtual Button* setCloseBtn(const std::string& path);

	virtual bool onKeyBack();

protected:
	virtual void initAdapter();

	bool _bIsSingleton;//�Ƿ�Ψһ
	bool _bIsAutoClose;//�Ƿ��Զ��ر� -- ��Ҫ��
	
	bool _bIsListenBackKey;//�Ƿ�������ؼ�
	bool _bIsTapBackGroundClose; // �Ƿ��������ر�
	
	DialogAdapter* _pPreviousDialog;//��ѹ��Ľ���
};


class DialogHelper
{
public:
	DialogHelper();
	~DialogHelper();
	
	static DialogAdapter* ShowUI(const std::string& filepath,bool push = true);
	static DialogAdapter* CloseUI(const std::string& filepath);
	static DialogAdapter* CloseUI(DialogAdapter* dialog);
	static DialogAdapter* CloseUI();
	static bool onKeyBack();
private:

};



#endif /* defined(__DIALOGADAPTER_H__) */
