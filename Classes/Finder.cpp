#include "Finder.h"



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
