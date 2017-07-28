#include "PageAdapter.h"
#include "DialogAdapter.h"


PageAdapter::PageAdapter()
{

}

PageAdapter::~PageAdapter()
{

}

PageAdapter* PageAdapter::create(const std::string& filename)
{
	PageAdapter *pRet = new(std::nothrow) PageAdapter();
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

PageAdapter* PageAdapter::create(Node* node)
{

	PageAdapter *pRet = new(std::nothrow) PageAdapter();
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

bool PageAdapter::init(const std::string& filename)
{
	bool bRet = false;

	do
	{
		CC_BREAK_IF(!FinderAdapter::init(this));
		CC_BREAK_IF(!Layout::init());
	
		_pSource = CSLoader::createNode(filename);
		_sSourceFileName = filename;
		_pSource->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		addChild(_pSource);
		this->setContentSize(Director::getInstance()->getVisibleSize());
		setSourcePositionNormalized(Vec2::ANCHOR_MIDDLE);
		initAdapter();

		bRet = true;
	} while (0);

	return  bRet;
}

bool PageAdapter::init(Node* node)
{

	bool bRet = false;

	do
	{
		CC_BREAK_IF(!FinderAdapter::init(this));
		CC_BREAK_IF(!Layout::init());

		_pSource = node;
		_pSource->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_sSourceFileName = _pSource->getName();
		addChild(_pSource);
		this->setContentSize(Director::getInstance()->getVisibleSize());
		setSourcePositionNormalized(Vec2::ANCHOR_MIDDLE);
		initAdapter();
		bRet = true;
	} while (0);

	return  bRet;
}


void PageAdapter::initAdapter()
{

}


void PageAdapter::setSourcePositionNormalized(const Vec2& pos)
{
	_pSource->setPositionNormalized(pos);
}

void PageAdapter::runTimeLineAction(Action* action)
{
	_pSource->runAction(action);
}

Node* PageAdapter::getChildByPath(const std::string& path)
{
	auto node = Finder::getChildByPath(this, path);
	if (node == nullptr){
		node = Finder::getChildByPath(_pSource, path);
	}
	return node;
}



Node* PageAdapter::getSource()
{
	getChildByPath<Node*>("");
	 //utils::findChild<Label*>(_pRoot, "");
	return _pSource;
}


std::string PageAdapter::getSourceName()
{
	return _sSourceFileName;
}

DialogAdapter* PageAdapter::showDialog(const std::string& filename)
{
	for (auto dialog : _dialogs) {
		if (dialog->getSourceName() == filename) {
			return static_cast<DialogAdapter*>(dialog);
		}
	}
	auto dialog = DialogAdapter::create(filename);
	_dialogs.pushBack(dialog);

	return dialog;
}

DialogAdapter* PageAdapter::showDialog(DialogAdapter* dialog)
{
// 	for (DialogAdapter dialog : dialogs) {
// 		if (dialog.getClass().equals(dialogAdapter.getClass())) {
// 			return dialogAdapter;
// 		}
// 	}

	return nullptr;
}

bool PageAdapter::onKeyBack()
{
	return false;
}
