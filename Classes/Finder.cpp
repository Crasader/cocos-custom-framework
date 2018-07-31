#include "Finder.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


bool FinderAdapter::init(Node* rSource)
{
	_pRoot = rSource;
	return true;
}

Node* FinderAdapter::getChildByPath(const std::string& path)
{
	auto node = Finder::getChildByPath(_pRoot, path);

	return node;
}

Widget* FinderAdapter::initButtonStyle(const std::string& path)
{

	float anchorX = 0.5f;
	float anchorY = 0.5f;



	auto _node = getChildByPath(path);
	auto node = static_cast<Widget*>(_node);
	
	if (node){
		initButtonStyle(node);
	}
	else{
		CCLOG("the [ %s ] bu cun zai ,huo zhe is not a widget", path.c_str());
		CCASSERT(node != nullptr, "node bu cun zai ,huo zhe is not a widget");
	}
	return node;
}

Widget* FinderAdapter::initButtonStyle(Widget* widget)
{
	float anchorX = 0.5f;
	float anchorY = 0.5f;

	auto node = widget;
	auto cacheAR = node->getAnchorPoint();
	auto diffX = (anchorX - cacheAR.x) * node->getContentSize().width  * (node->getScaleX());
	auto diffY = (anchorY - cacheAR.y) * node->getContentSize().height * (node->getScaleY());

	node->setAnchorPoint(Vec2(anchorX, anchorY));
	node->setPositionX(node->getPositionX() + diffX);
	node->setPositionY(node->getPositionY() + diffY);

	node->addTouchEventListener([=](Ref* sender, Widget::TouchEventType eventType){
		auto btn = static_cast<Widget*>(sender);
		float pressedScale = 1.1f;
		Action* touchAction = nullptr;
		switch (eventType)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			btn->setScale(pressedScale);
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:

			btn->setScale(btn->isHighlighted() ? pressedScale : 1);

			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			btn->setScale(1);
			break;
		default:
			break;
		}
	});
	return node;
}

Node* Finder::getChildByPath(Node* root, const std::string& path)
{
	std::vector<std::string> vec = split(path, "/");

	Node* node = nullptr;

	for (auto str : vec)
	{
		if (node == nullptr){
			node = root->getChildByName(str);
			if (node == nullptr)break;
		}
		else{
			node = node->getChildByName(str);
			if (node == nullptr)break;
		}
	}

	return node;
}

std::vector<std::string> Finder::split(const std::string & _str, const std::string & separator)
{

	std::string str = _str;
	std::vector<std::string> result;
	int cutAt;
	while ((cutAt = str.find_first_of(separator)) != str.npos)
	{
		if (cutAt > 0)
		{
			result.push_back(str.substr(0, cutAt));
		}
		str = str.substr(cutAt + 1);
	}
	if (str.length() > 0)
	{
		result.push_back(str);
	}

	return result;
}
