#include "BaseDialog.h"
#include "VisibleRect.h"
static std::vector<std::string> _vUiStrVec;
static Vector<Layout *> _vUiVec;

BaseDialog::BaseDialog() :
_fDelayAutoClose(2.0f),
_bIsAutoClose(false),
_bIsSingleton(true),
_bIsListenBackKey(true),
_bIsTapBackGroundClose(false),
_pPreviousDialog(nullptr)
{

}

BaseDialog::~BaseDialog()
{

}
BaseDialog* BaseDialog::create(const std::string& filename)
{
	BaseDialog *pRet = new(std::nothrow) BaseDialog();
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

BaseDialog* BaseDialog::create(Node* node)
{
	BaseDialog *pRet = new(std::nothrow) BaseDialog();
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

bool BaseDialog::initWithParam(Ref* param)
{
	return true;
}

bool BaseDialog::initWithNoParam()
{
	return true;
}

void BaseDialog::initAdapter()
{
	setMaskColor();
}

Button* BaseDialog::setCloseBtn(const std::string& path)
{
	return setBtnClickListener(path, [=](Ref* sender){
		close();
	});
}

Button* BaseDialog::setBtnClickListener(const std::string& path, const ccWidgetClickCallback& _clickCallBack)
{
	auto btn = getChildByPath<Button*>(path);
	btn->addClickEventListener(_clickCallBack);
	return btn;
}

BaseDialog* BaseDialog::setMaskColor(const Color4F& color)
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

void BaseDialog::show()
{
	doShowAction();
}

void BaseDialog::close()
{
	DialogHelper::CloseUI(this);
}

float BaseDialog::doShowAction()
{
	_pRoot->stopAllActions();
	float duration = getShowDuration();
	_pRoot->setScale(0.2f);
	_pRoot->setOpacity(0);
	_pRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_pRoot->runAction(Spawn::create(FadeIn::create(duration), ScaleTo::create(duration, 1), nullptr));

	this->setOpacity(0);
	this->runAction(FadeIn::create(duration/2));

	return duration;
}

float BaseDialog::doCloseAction()
{
	_pRoot->stopAllActions();
	float duration = getCloseDuration();
	_pRoot->runAction(Sequence::create(Spawn::create(FadeOut::create(duration), ScaleTo::create(duration, 0.2f), nullptr),nullptr));
	this->runAction(FadeOut::create(duration));

	return duration;
}

float BaseDialog::getShowDuration()
{
	return 0.2f;
}

float BaseDialog::getCloseDuration()
{
	return 0.2f;
}

float BaseDialog::behindScene()
{
	_pRoot->stopAllActions();
	float duration = 0.25f;
	_pRoot->runAction(Spawn::create(FadeOut::create(duration), ScaleTo::create(duration, 0.2f), nullptr));

	return duration;
}

float BaseDialog::recoverScene()
{
	_pRoot->stopAllActions();
	float duration = 0.25f;
	_pRoot->runAction(Spawn::create(FadeIn::create(duration), ScaleTo::create(duration, 1), nullptr));
	
	return duration;
}

BaseDialog* BaseDialog::setSingleton(bool bSingleton)
{
	_bIsSingleton = bSingleton;
	return this;
}

bool BaseDialog::isSingleton()
{
	return _bIsSingleton;
}


BaseDialog* BaseDialog::setListenBackKey(bool bIsListenBackKey)
{
	_bIsListenBackKey = bIsListenBackKey;
	return this;
}

bool BaseDialog::isListenBackKey()
{
	return _bIsListenBackKey;
}
BaseDialog* BaseDialog::setTapBackGroundClose(bool bTapBackGroundClose)
{
	_bIsTapBackGroundClose = bTapBackGroundClose;
	return this;
}

bool BaseDialog::isTapBackGroundClose()
{
	return _bIsTapBackGroundClose;
}
void BaseDialog::setPreviousDialog(BaseDialog* rPreviousDialog)
{
	_pPreviousDialog = rPreviousDialog;
}

BaseDialog* BaseDialog::getPreviousDialog()
{
	return _pPreviousDialog;
}

bool BaseDialog::onKeyBack()
{
	if (_bIsListenBackKey){
		close();
	}
	return true;
}


void BaseDialog::setAutoCloseDelay(float delay)
{
	_fDelayAutoClose = delay;
}

float BaseDialog::getAutoCloseDelay()
{
	return _fDelayAutoClose;
}
BaseDialog* BaseDialog::setAutoClose(bool bAutoClose)
{
	if (_bIsAutoClose != bAutoClose){
		_bIsAutoClose = bAutoClose;

		if (_bIsAutoClose){
			this->runAction(Sequence::create(DelayTime::create(getAutoCloseDelay()), CallFunc::create([=](){
				close();
			}), nullptr));
		}
	}
	
	return this;
}

bool BaseDialog::isAutoClose()
{
	return _bIsAutoClose;
}

void BaseDialog::onShowCallback()
{
	if (_showCallbackFunc){
		_showCallbackFunc();
	}
}

void BaseDialog::onCloseCallback()
{
	if (_closeCallbackFunc){
		_closeCallbackFunc();
	}
}

void BaseDialog::setShowCallbackFunc(std::function<void()> showCallbackFunc)
{
	_showCallbackFunc = showCallbackFunc;
}

void BaseDialog::setCloseCallbackFunc(std::function<void()> closeCallbackFunc)
{
	_closeCallbackFunc = closeCallbackFunc;
}

void BaseDialog::onBehindSceneCallback()
{
	if (_behindSceneCallbackFunc){
		_behindSceneCallbackFunc();
	}
}

void BaseDialog::onRecoverSceneCallback()
{
	if (_recoverSceneCallbackFunc){
		_recoverSceneCallbackFunc();
	}
}

void BaseDialog::setBehindSceneCallback(std::function<void()> behindSceneCallbackFunc)
{
	_behindSceneCallbackFunc = behindSceneCallbackFunc;
}

void BaseDialog::setRecoverSceneCallback(std::function<void()> recoverSceneCallbackFunc)
{
	_recoverSceneCallbackFunc = recoverSceneCallbackFunc;
}


std::vector<Node*> BaseDialog::getNodeVector(const char* name, ...)
{
	va_list args;

	va_start(args, name);
	auto nodes = getNodeVectorFrom_va_list(name, args);
	va_end(args);

	return nodes;
}

std::vector<Node*> BaseDialog::getNodeVectorFrom_va_list(const char* name, va_list args)
{
	std::vector<Node*> nodes;
	const char* now = name;
	auto node = getChildByPath(now);
	bool bNext = true;
	while (bNext)
	{
		nodes.push_back(node);
		now = va_arg(args, char*);
		if (now != nullptr){
			node = getChildByPath(now);
			if (node == nullptr){
				bNext = false;
			}
		}
		else{
			bNext = false;
		}
	}
 	return nodes;
}

void BaseDialog::showButtonDelayPop(const char* name, ...)
{
	va_list args;
	va_start(args, name);
	auto nodes = getNodeVectorFrom_va_list(name, args);
	va_end(args);

	showButtonDelayPop(nodes);
}

void BaseDialog::showButtonDelayPop(Node* node, ...)
{
	va_list args;
	va_start(args, node);
	std::vector<Node*> nodes;
	Node* now = node;
	bool bNext = true;
	while (bNext)
	{
		nodes.push_back(now);
		now = va_arg(args, Node*);
		if (now == nullptr){
			bNext = false;
		}
	}
	va_end(args);

	showButtonDelayPop(nodes);
}

void BaseDialog::showButtonDelayPop(std::vector<Node*> btns)
{
	showButtonDelayPop(btns,0.6f,0.4f);
}

void BaseDialog::showButtonDelayPop(std::vector<Node*> btns, float delay, float duration)
{
	for (int i = 0; i < btns.size();i++)
	{
		auto node = btns.at(i);
		node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		node->setOpacity(0);
		node->setScale(0);
		node->runAction(Sequence::create(DelayTime::create(delay*i+0.2f), Spawn::create(FadeIn::create(duration), EaseBackOut::create(ScaleTo::create(duration, 1)), nullptr), nullptr));
	}
}


BaseDialog* DialogHelper::createUIWithParam(const std::string& filepath, Ref* sender)
{
	BaseDialog* dialog = static_cast<BaseDialog*>(ObjectFactory::getInstance()->createObject(filepath));
	if (dialog == nullptr){
		CCLOG("ObjectFactory can't create the UI-> [ %s ], use default constructor.", filepath.c_str());
		dialog = BaseDialog::create(filepath);
	}
	if (sender){
		if (dialog && dialog->initWithParam(sender)){
			return dialog;
		}
	}
	return dialog;
}

BaseDialog* DialogHelper::ShowUI(const std::string& filepath, bool push /*= true*/)
{
	return ShowUI(filepath,nullptr,push);
}

BaseDialog* DialogHelper::ShowUI(const std::string& filepath, Ref* param, bool push /*= true*/)
{

	auto scene = Director::getInstance()->getRunningScene();
	auto existdialog = static_cast<BaseDialog*>(scene->getChildByName(filepath));//用作判断是否存在
	BaseDialog* dialog = nullptr;
	bool bCreateNewDialog = true;//是否 新建 Dialog
	if (existdialog){
		bCreateNewDialog = existdialog->isSingleton() == false;//如果不是单例则新建
	}
	if (bCreateNewDialog){//当前界面已经显示了要显示的 dialog

		dialog = createUIWithParam(filepath,param);
		dialog->setName(filepath);
		scene->addChild(dialog);
		float duration = dialog->doShowAction();
		dialog->runAction(Sequence::create(DelayTime::create(duration), CallFuncN::create([=](Node* node){//显示动作执行完之后的回调
			auto _BaseDialog = static_cast<BaseDialog*>(node);
			_BaseDialog->onShowCallback();
		}), nullptr));

		if (push){
			if (_vUiVec.empty() == false){
				auto thelastdialog = static_cast<BaseDialog*>(_vUiVec.back());
				if (thelastdialog && thelastdialog->isAutoClose() == false){//压入后台
					dialog->setPreviousDialog(thelastdialog);
					float dur = thelastdialog->behindScene();
					thelastdialog->runAction(Sequence::create(DelayTime::create(dur), CallFuncN::create([=](Node* node){
						auto _BaseDialog = static_cast<BaseDialog*>(node);
						_BaseDialog->onBehindSceneCallback();//压入后台之后的回调
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

BaseDialog* DialogHelper::CloseUI()
{
	BaseDialog* dialog = static_cast<BaseDialog*>(_vUiVec.back());
	return CloseUI(dialog);
}

BaseDialog* DialogHelper::CloseUI(const std::string& filepath)
{
	BaseDialog* dialog = nullptr;
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
		dialog = static_cast<BaseDialog*>(scene->getChildByName(filepath));
		if (dialog){
			auto rPreviousDialog = dialog->getPreviousDialog();
			if (rPreviousDialog){//恢复到场景
				float dur = rPreviousDialog->recoverScene();
				rPreviousDialog->runAction(Sequence::create(DelayTime::create(dur), CallFuncN::create([=](Node* node){
					auto _BaseDialog = static_cast<BaseDialog*>(node);
					_BaseDialog->onRecoverSceneCallback();//恢复完成之后的回调
				}), nullptr));
			}
			float duration = dialog->doCloseAction();
			dialog->runAction(Sequence::create(DelayTime::create(duration), CallFuncN::create([=](Node* node){
				auto _BaseDialog = static_cast<BaseDialog*>(node);
				_BaseDialog->onCloseCallback();
			}), RemoveSelf::create(), nullptr));
		}
	}
	return dialog;
}

BaseDialog* DialogHelper::CloseUI(BaseDialog* dialog)
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
				auto _BaseDialog = static_cast<BaseDialog*>(node);
				_BaseDialog->onRecoverSceneCallback();//恢复完成之后的回调
			}), nullptr));
		}
		float duration = dialog->doCloseAction();
		dialog->runAction(Sequence::create(DelayTime::create(duration), CallFuncN::create([=](Node* node){
			auto _BaseDialog = static_cast<BaseDialog*>(node);
			_BaseDialog->onCloseCallback();
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
	auto dialog = static_cast<BaseDialog*>(temdialog);
	return dialog->onKeyBack();
}

void DialogHelper::cleanDialog()
{
	_vUiVec.clear();
	_vUiStrVec.clear();
}

int DialogHelper::getSize()
{
	return _vUiVec.size();
}
