#include "BasePage.h"
#include "BaseDialog.h"


BasePage::BasePage()
{

}

BasePage::~BasePage()
{

}

BasePage* BasePage::create(const std::string& filename)
{
	BasePage *pRet = new(std::nothrow) BasePage();
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

BasePage* BasePage::create(Node* node)
{

	BasePage *pRet = new(std::nothrow) BasePage();
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

bool BasePage::init(const std::string& filename)
{
	bool bRet = false;

	do
	{
		CC_BREAK_IF(!FinderAdapter::init(this));
		CC_BREAK_IF(!Layout::init());
	
		_pRoot = CSLoader::createNode(filename);
		_sRootFileName = filename;
		_pRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_pRoot->setName("Root");
		addChild(_pRoot);
		this->setContentSize(Director::getInstance()->getWinSize());
		_pRoot->setPosition(this->getContentSize() / 2);
		registerEvent();
		initAdapter();

		bRet = true;
	} while (0);

	return  bRet;
}

bool BasePage::init(Node* node)
{

	bool bRet = false;

	do
	{
		CC_BREAK_IF(!FinderAdapter::init(this));
		CC_BREAK_IF(!Layout::init());

		_pRoot = node;
		_pRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_sRootFileName = _pRoot->getName();
		_pRoot->setName("Root");
		addChild(_pRoot);
		this->setContentSize(Director::getInstance()->getWinSize());
		_pRoot->setPosition(this->getContentSize() / 2);
		registerEvent();
		initAdapter();
		bRet = true;
	} while (0);

	return  bRet;
}


void BasePage::initAdapter()
{

}


void BasePage::runTimeLineAction(Action* action)
{
	_pRoot->runAction(action);
}

void BasePage::registerEvent()
{

}

Node* BasePage::getChildByPath(const std::string& path)
{
	auto node = Finder::getChildByPath(this, path);
	if (node == nullptr){
		node = Finder::getChildByPath(_pRoot, path);
	}
	return node;
}



Node* BasePage::getRoot()
{
	return _pRoot;
}


std::string BasePage::getRootName()
{
	return _sRootFileName;
}


bool BasePage::onKeyBack()
{
	return false;
}
