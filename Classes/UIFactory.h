#ifndef __UIFactory_H__
#define __UIFactory_H__

#include "PublicDefine.h"


class UIFactory :public Ref
{
public:

	CREATE_INSTANCE_FUNC(UIFactory);
	virtual bool init();
	
	Node* createUI(const std::string& str);
protected:
	 
private:
	UIFactory(){}

	UIFactory(const UIFactory&) = delete;

	UIFactory &operator=(const UIFactory&) = delete;

};

#endif /* defined(__UIFactory_H__) */
