#ifndef __Finder_H__
#define __Finder_H__

#include "cocos2d.h"
USING_NS_CC;

class FinderAdapter{
public:
	bool init(Node* rSource);
	virtual Node* getChildByPath(const std::string& path);

private:
	Node* _pRoot = nullptr;
};
class Finder
{
public:
	static Node* getChildByPath(Node* root, const std::string& path);
	template<typename T> inline
		static T getChildByPath(Node* root, const std::string& path)
	{
			return dynamic_cast<T>(getChildByPath(root,path));
		}
private:
	static std::vector<std::string> split(const std::string & str, const std::string & separator);

};

#endif /* defined(__Finder_H__) */
