#include "DialogAdapter.h"
#include "VisibleRect.h"
static vector<std::string> _vUiStrVec;
static Vector<Layout *> _vUiVec;

DialogAdapter::DialogAdapter() :
_bIsAutoClose(false),
_bIsSingleton(true),
_bIsListenBackKey(true),
_bIsTapBackGroundClose(false),
_pPreviousDialog(nullptr)
{

}

DialogAdapter::~DialogAdapter()
{

}

DialogAdapter* DialogAdapter::create(const std::string& filename)
{
	DialogAdapter *pRet = new(std::nothrow) DialogAdapter();
	if (pRet && pRet->init(filename))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

DialogAdapter* DialogAdapter::create(Node* node)
{
	DialogAdapter *pRet = new(std::nothrow) DialogAdapter();
	if (pRet && pRet->init(node))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

void DialogAdapter::initAdapter()
{
	setMaskColor();
}

Button* DialogAdapter::setCloseBtn(const std::string& path)
{
	return setBtnClickCallback(path, [=](Ref* sender){
		close();
	});
}

Button* DialogAdapter::setBtnClickCallback(const std::string& path, const ccWidgetClickCallback& _clickCallBack)
{
	auto btn = getChildByPath<Button*>(path);
	btn->addClickEventListener(_clickCallBack);
	return btn;
}

DialogAdapter* DialogAdapter::setMaskColor(const Color4F& color)
{
	Layout* _layout = this;
	_layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	_layout->setBackGroundColor(Color3B(color));
	_layout->setBackGroundColorOpacity(color.a * 255);
	_layout->setTouchEnabled(true);

	_layout->addClickEventListener([=](Ref* sender){
		if (_bIsTapBackGroundClose){
			close();
		}
	});

	return this;
}

void DialogAdapter::show()
{
	doShowAction();
}

void DialogAdapter::close()
{
	DialogHelper::CloseUI(this);
}

float DialogAdapter::doShowAction()
{
	_pSource->stopAllActions();
	float duration = getShowDuration();
	_pSource->setScale(0.2f);
	_pSource->setOpacity(0);
	_pSource->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_pSource->runAction(Spawn::create(FadeIn::create(duration), ScaleTo::create(duration, 1), nullptr));

	this->setOpacity(0);
	this->runAction(FadeIn::create(duration/2));

	return duration;
}

float DialogAdapter::doCloseAction()
{
	_pSource->stopAllActions();
	float duration = getCloseDuration();
	_pSource->runAction(Sequence::create(Spawn::create(FadeOut::create(duration), ScaleTo::create(duration, 0.2f), nullptr),nullptr));
	this->runAction(FadeOut::create(duration));

	return duration;
}

float DialogAdapter::getShowDuration()
{
	return 0.2f;
}

float DialogAdapter::getCloseDuration()
{
	return 0.2f;
}

float DialogAdapter::behindScene()
{
	_pSource->stopAllActions();
	float duration = 0.25f;
	_pSource->runAction(Spawn::create(FadeOut::create(duration), ScaleTo::create(duration, 0.2f), nullptr));

	return duration;
}

float DialogAdapter::recoverScene()
{
	_pSource->stopAllActions();
	float duration = 0.25f;
	_pSource->runAction(Spawn::create(FadeIn::create(duration), ScaleTo::create(duration, 1), nullptr));
	
	return duration;
}

DialogAdapter* DialogAdapter::setSingleton(bool bSingleton)
{
	_bIsSingleton = bSingleton;
	return this;
}

bool DialogAdapter::isSingleton()
{
	return _bIsSingleton;
}


DialogAdapter* DialogAdapter::setListenBackKey(bool bIsListenBackKey)
{
	_bIsListenBackKey = bIsListenBackKey;
	return this;
}

bool DialogAdapter::isListenBackKey()
{
	return _bIsListenBackKey;
}
DialogAdapter* DialogAdapter::setTapBackGroundClose(bool bTapBackGroundClose)
{
	_bIsTapBackGroundClose = bTapBackGroundClose;
	return this;
}

bool DialogAdapter::isTapBackGroundClose()
{
	return _bIsTapBackGroundClose;
}
void DialogAdapter::setPreviousDialog(DialogAdapter* rPreviousDialog)
{
	_pPreviousDialog = rPreviousDialog;
}

DialogAdapter* DialogAdapter::getPreviousDialog()
{
	return _pPreviousDialog;
}

bool DialogAdapter::onKeyBack()
{
	if (_bIsListenBackKey){
		close();
	}
	return true;
}

DialogAdapter* DialogAdapter::setAutoClose(bool bAutoClose)
{
	if (_bIsAutoClose != bAutoClose){
		_bIsAutoClose = bAutoClose;

		if (_bIsAutoClose){
			this->runAction(Sequence::create(DelayTime::create(2.5f), CallFunc::create([=](){
				close();
			}), nullptr));
		}
	}
	
	return this;
}

bool DialogAdapter::isAutoClose()
{
	return _bIsAutoClose;
}

void DialogAdapter::onShowCallback()
{

}

void DialogAdapter::onCloseCallback()
{

}

void DialogAdapter::onBehindSceneCallback()
{

}

void DialogAdapter::onRecoverSceneCallback()
{

}
void DialogAdapter::registerEventDispatcher()
{

}


DialogAdapter* DialogHelper::ShowUI(const std::string& filepath, bool push/* = true*/)
{

	auto scene = Director::getInstance()->getRunningScene();
	auto existdialog = static_cast<DialogAdapter*>(scene->getChildByName(filepath));//用作判断是否存在
	DialogAdapter* dialog = nullptr;
	bool bCreateNewDialog = true;//是否 新建 Dialog
	if (existdialog){
		bCreateNewDialog = existdialog->isSingleton() == false;//如果不是单例则新建
	}
	if (bCreateNewDialog){//当前界面已经显示了要显示的 dialog
		
		dialog = static_cast<DialogAdapter*>(ObjectFactory::getInstance()->createObject(filepath));
		if (dialog == nullptr){
			CCLOG("UIFactory can't create the UI->%s", filepath.c_str());
			dialog = DialogAdapter::create(filepath);
		}
		dialog->setName(filepath);
		scene->addChild(dialog);
		float duration = dialog->doShowAction();
		dialog->runAction(Sequence::create(DelayTime::create(duration), CallFuncN::create([=](Node* node){//显示动作执行完之后的回调
			auto dialogadapter = static_cast<DialogAdapter*>(node);
			dialogadapter->onShowCallback();
		}),nullptr));

		if (push){
			if (_vUiVec.empty() == false){
				auto thelastdialog = static_cast<DialogAdapter*>(_vUiVec.back());
				if (thelastdialog && thelastdialog->isAutoClose() == false){//压入后台
					dialog->setPreviousDialog(thelastdialog);
					float dur = thelastdialog->behindScene();
					thelastdialog->runAction(Sequence::create(DelayTime::create(dur), CallFuncN::create([=](Node* node){
						auto dialogadapter = static_cast<DialogAdapter*>(node);
						dialogadapter->onBehindSceneCallback();//压入后台之后的回调
					}), nullptr));
				}
			}
		}
		_vUiStrVec.push_back(filepath);
		_vUiVec.pushBack(dialog);
	}
	else{
		dialog = existdialog;
	}
	return dialog;
}
DialogAdapter* DialogHelper::CloseUI()
{
	DialogAdapter* dialog = static_cast<DialogAdapter*>(_vUiVec.back());
	return CloseUI(dialog);
}

DialogAdapter* DialogHelper::CloseUI(const std::string& filepath)
{
	DialogAdapter* dialog = nullptr;
	bool isExist = false;

	int keyindex = _vUiStrVec.size();
	for (int i = _vUiStrVec.size() - 1; i >= 0; i--)
	{
		if (_vUiStrVec.at(i) == filepath)
		{
			keyindex = i + 1;
			isExist = true;
			break;
		}
	}
	_vUiVec.erase(keyindex - 1);
	_vUiStrVec.erase(_vUiStrVec.begin() + keyindex - 1);

	auto scene = Director::getInstance()->getRunningScene();

	if (isExist){
		dialog = static_cast<DialogAdapter*>(scene->getChildByName(filepath));
		if (dialog){
			auto rPreviousDialog = dialog->getPreviousDialog();
			if (rPreviousDialog){//恢复到场景
				float dur = rPreviousDialog->recoverScene();
				rPreviousDialog->runAction(Sequence::create(DelayTime::create(dur), CallFuncN::create([=](Node* node){
					auto dialogadapter = static_cast<DialogAdapter*>(node);
					dialogadapter->onRecoverSceneCallback();//恢复完成之后的回调
				}), nullptr));
			}
			float duration = dialog->doCloseAction();
			dialog->runAction(Sequence::create(DelayTime::create(duration), CallFuncN::create([=](Node* node){
				auto dialogadapter = static_cast<DialogAdapter*>(node);
				dialogadapter->onCloseCallback();
			}), RemoveSelf::create(), nullptr));
		}
	}
	return dialog;
}

DialogAdapter* DialogHelper::CloseUI(DialogAdapter* dialog)
{
	bool isExist = false;
	auto it = find(_vUiVec.begin(), _vUiVec.end(), dialog);
	if (it != _vUiVec.end()){
		isExist = true;
		int keyindex = _vUiVec.size();
		for (int i = 0; i < _vUiVec.size(); i++)
		{
			if (_vUiVec.at(i) == dialog)
			{
				keyindex = i + 1;
				break;
			}
		}
		_vUiVec.erase(keyindex - 1);
		_vUiStrVec.erase(_vUiStrVec.begin() + keyindex - 1);
	}


	if (isExist){
		auto rPreviousDialog = dialog->getPreviousDialog();
		if (rPreviousDialog){//恢复到场景
			float dur = rPreviousDialog->recoverScene();
			rPreviousDialog->runAction(Sequence::create(DelayTime::create(dur), CallFuncN::create([=](Node* node){
				auto dialogadapter = static_cast<DialogAdapter*>(node);
				dialogadapter->onRecoverSceneCallback();//恢复完成之后的回调
			}), nullptr));
		}
		float duration = dialog->doCloseAction();
		dialog->runAction(Sequence::create(DelayTime::create(duration), CallFuncN::create([=](Node* node){
			auto dialogadapter = static_cast<DialogAdapter*>(node);
			dialogadapter->onCloseCallback();
		}), RemoveSelf::create(), nullptr));
	}
	return dialog;
}

bool DialogHelper::onKeyBack()
{
	if (_vUiVec.empty()){
		return false;
	}

	auto temdialog = _vUiVec.back();
	auto dialog = static_cast<DialogAdapter*>(temdialog);
	return dialog->onKeyBack();
}
