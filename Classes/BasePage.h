#ifndef __BasePage_H__
#define __BasePage_H__

#include "PublicCoco.h"
#include "Finder.h"

class BaseDialog;
class BasePage : public Layout, public FinderAdapter
{
public:
	BasePage();
	~BasePage();

	static BasePage* create(const std::string& filename);
	static BasePage* create(Node* node);
	virtual bool init(const std::string& filename);
	virtual bool init(Node* node);		

	
	virtual void runTimeLineAction(Action* action);


	virtual Node* getChildByPath(const std::string& path);
 	template<typename T> T getChildByPath(const std::string& name)
 	{
 		return dynamic_cast<T>(getChildByPath(name));
 	}

	virtual Node* getRoot();
	virtual std::string getRootName();

	virtual bool onKeyBack();

public:

protected:
	virtual void initAdapter();
	virtual void registerEvent();
protected:
	Node* _pRoot = nullptr;
	std::string _sRootFileName;
};

#endif /* defined(__BasePage_H__) */
