
#include "myObjectFactory.h"


NS_CC_BEGIN
namespace c2d{

	myObjectFactory::TInfo::TInfo(void)
		:_class("")
		, _fun(nullptr)
		, _func(nullptr)
		, _fun_param(nullptr)
		, _func_param(nullptr)
	{
	}

	myObjectFactory::TInfo::TInfo(const std::string& type, Instance ins)
		:_class(type)
		, _fun(ins)
		, _func(nullptr)
		, _fun_param(nullptr)
		, _func_param(nullptr)
	{
		myObjectFactory::getInstance()->registerType(*this);
	}

	myObjectFactory::TInfo::TInfo(const std::string& type, InstanceFunc ins)
		:_class(type)
		, _fun(nullptr)
		, _func(ins)
		, _fun_param(nullptr)
		, _func_param(nullptr)
	{
		myObjectFactory::getInstance()->registerType(*this);
	}

	myObjectFactory::TInfo::TInfo(const std::string& type, InstanceParam ins)
		:_class(type)
		, _fun(nullptr)
		, _func(nullptr)
		, _fun_param(ins)
		, _func_param(nullptr)
	{
		myObjectFactory::getInstance()->registerType(*this);
	}

	myObjectFactory::TInfo::TInfo(const std::string& type, InstanceParamFunc ins)
		:_class(type)
		, _fun(nullptr)
		, _func(nullptr)
		, _fun_param(nullptr)
		, _func_param(ins)
	{
		myObjectFactory::getInstance()->registerType(*this);
	}

	myObjectFactory::TInfo::TInfo(const TInfo &t)
	{
		_class = t._class;
		_fun = t._fun;
		_func = t._func;

		_fun_param = t._fun_param;
		_func_param = t._func_param;
	}


	myObjectFactory::TInfo::~TInfo(void)
	{
		_class = "";
		_fun = nullptr;
		_func = nullptr;
		_fun_param = nullptr;
		_func_param = nullptr;
	}

	myObjectFactory::TInfo& myObjectFactory::TInfo::operator= (const TInfo &t)
	{
		_class = t._class;
		_fun = t._fun;
		_func = t._func;

		_fun_param = t._fun_param;
		_func_param = t._func_param;
		return *this;
	}


	myObjectFactory* myObjectFactory::_sharedFactory = nullptr;

	myObjectFactory::myObjectFactory(void)
	{

	}

	myObjectFactory::~myObjectFactory(void)
	{
		_typeMap.clear();
	}

	myObjectFactory* myObjectFactory::getInstance()
	{
		if (nullptr == _sharedFactory)
		{
			_sharedFactory = new (std::nothrow) myObjectFactory();
		}
		return _sharedFactory;
	}

	void myObjectFactory::destroyInstance()
	{
		CC_SAFE_DELETE(_sharedFactory);
	}

	cocos2d::Ref* myObjectFactory::createObject(const std::string &name, cocos2d::Ref* var)
	{
		Ref *o = nullptr;
		do
		{
			const TInfo t = _typeMap[name];
			if (var == nullptr){//无参数
				if (t._fun != nullptr)
				{
					o = t._fun();
				}
				else if (t._func != nullptr)
				{
					o = t._func();
				}
			}
			else//有参数
			{
				if (t._fun_param != nullptr)
				{
					o = t._fun_param(var);
				}
				else if (t._func_param != nullptr)
				{
					o = t._func_param(var);
				}
			}
		
		} while (0);

		return o;
	}

	void myObjectFactory::registerType(const TInfo &t)
	{
		_typeMap.insert(std::make_pair(t._class, t));
	}

}
NS_CC_END
