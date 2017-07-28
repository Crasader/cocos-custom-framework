#ifndef __PAGEADAPTER_H__
#define __PAGEADAPTER_H__

#include "PublicCoco.h"
#include "Finder.h"

class DialogAdapter;
class PageAdapter : public Layout, public FinderAdapter
{
public:
	PageAdapter();
	~PageAdapter();

	static PageAdapter* create(const std::string& filename);
	static PageAdapter* create(Node* node);
	virtual bool init(const std::string& filename);
	virtual bool init(Node* node);
	virtual DialogAdapter* showDialog(const std::string& filename);
	virtual DialogAdapter* showDialog(DialogAdapter* dialog);

		

	virtual void setSourcePositionNormalized(const Vec2& pos);
	virtual void runTimeLineAction(Action* action);


	virtual Node* getChildByPath(const std::string& path);
 	template<typename T> T getChildByPath(const std::string& name)
 	{
 		return dynamic_cast<T>(getChildByPath(name));
 	}

	virtual Node* getSource();
	virtual std::string getSourceName();

	virtual bool onKeyBack();
public:

protected:
	virtual void initAdapter();
	
protected:
	Node* _pSource = nullptr;
	std::string _sSourceFileName;
	Vector<PageAdapter*> _dialogs;

};

#endif /* defined(__PAGEADAPTER_H__) */
