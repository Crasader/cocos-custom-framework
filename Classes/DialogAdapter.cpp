#include "DialogAdapter.h"
static vector<std::string> _dialogStack;

DialogAdapter::DialogAdapter() :
_bIsAutoClose(false),
_bStar(true),
_bIsListenBackKey(true),
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
	_layout->setContentSize(Director::getInstance()->getVisibleSize());
	_layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	_layout->setBackGroundColor(Color3B(color));
	_layout->setBackGroundColorOpacity(color.a * 255);
	_layout->setTouchEnabled(true);

	_layout->addClickEventListener([=](Ref* sender){
		if (_bIsAutoClose){
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
	DialogHelper::CloseUI(_sSourceFileName);
}

void DialogAdapter::doShowAction()
{
	_pSource->stopAllActions();
	float duration = 0.2f;
	_pSource->setScale(0.2f);
	_pSource->setOpacity(0);
	_pSource->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_pSource->runAction(Spawn::create(FadeIn::create(duration), ScaleTo::create(duration, 1), nullptr));

	this->setOpacity(0);
	this->runAction(FadeIn::create(duration/2));
}

void DialogAdapter::doCloseAction()
{
	_pSource->stopAllActions();
	float duration = 0.2f;
	_pSource->runAction(Sequence::create(Spawn::create(FadeOut::create(duration), ScaleTo::create(duration, 0.2f), nullptr), CallFunc::create([=](){
		removeFromParent();
	}), nullptr));


	this->runAction(FadeOut::create(duration));
}

void DialogAdapter::behindScene()
{
	_pSource->stopAllActions();
	float duration = 0.25f;
	_pSource->runAction(Spawn::create(FadeOut::create(duration), ScaleTo::create(duration, 0.2f), nullptr));
}

void DialogAdapter::recoverScene()
{
	_pSource->stopAllActions();
	float duration = 0.25f;
	_pSource->runAction(Spawn::create(FadeIn::create(duration), ScaleTo::create(duration, 1), nullptr));
}

DialogAdapter* DialogAdapter::setStar(bool bStar)
{
	_bStar = bStar;
	return this;
}

bool DialogAdapter::isStar()
{
	return _bStar;
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
	close();
	return true;
}

DialogAdapter* DialogAdapter::setAutoClose(bool bAutoClose)
{
	_bIsAutoClose = bAutoClose;
	return this;
}

bool DialogAdapter::isAutoClose()
{
	return _bIsAutoClose;
}

DialogAdapter* DialogHelper::ShowUI(const std::string& filepath)
{

	auto scene = Director::getInstance()->getRunningScene();
	auto dialog = static_cast<DialogAdapter*>(scene->getChildByName(filepath));

	if (dialog == nullptr){//当前界面已经显示了要显示的 dialog
		dialog = static_cast<DialogAdapter*>(ObjectFactory::getInstance()->createObject(filepath));
		if (dialog == nullptr){
			CCLOG("UIFactory can't create the UI->%s", filepath.c_str());
			dialog = DialogAdapter::create(filepath);
		}
		dialog->setName(filepath);
		scene->addChild(dialog);

		dialog->doShowAction();

		if (dialog->isStar()){
			if (_dialogStack.empty() == false){
				auto thelastdialogname = _dialogStack.back();
				auto thelastdialog = static_cast<DialogAdapter*>(scene->getChildByName(filepath));
				if (thelastdialog){
					dialog->setPreviousDialog(thelastdialog);
					thelastdialog->behindScene();
				}
			}
		}

		_dialogStack.push_back(filepath);
	}


	return dialog;
}

DialogAdapter* DialogHelper::CloseUI(const std::string& filepath)
{

	auto it = find(_dialogStack.begin(), _dialogStack.end(), filepath);
	if (it != _dialogStack.end()){
		_dialogStack.erase(it);
	}


	auto scene = Director::getInstance()->getRunningScene();
	auto dialog = static_cast<DialogAdapter*>(scene->getChildByName(filepath));
	if (dialog){
		auto rPreviousDialog = dialog->getPreviousDialog();
		if (rPreviousDialog){
			rPreviousDialog->recoverScene();
		}

		dialog->doCloseAction();
	}
	return dialog;
}

bool DialogHelper::onKeyBack()
{
	if (_dialogStack.empty()){
		return false;
	}
	auto dialogStr = _dialogStack.back();
	auto dialog = CloseUI(dialogStr);
	return dialog->onKeyBack();
}
