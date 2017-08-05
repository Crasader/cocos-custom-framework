
#ifndef __MYOBJECTFACTORY_H__
#define __MYOBJECTFACTORY_H__

#include <string>
#include <unordered_map>
#include <functional>
#include "cocos2d.h"

NS_CC_BEGIN
namespace c2d{

	class  myObjectFactory
	{
	public:
		typedef cocos2d::Ref* (*Instance)(void);
		typedef std::function<cocos2d::Ref* (void)> InstanceFunc;

		typedef cocos2d::Ref* (*InstanceParam)(Ref *);
		typedef std::function<cocos2d::Ref* (Ref *)> InstanceParamFunc;
		struct  TInfo
		{
			TInfo(void);
			TInfo(const std::string& type, Instance ins = nullptr);
			TInfo(const std::string& type, InstanceFunc ins = nullptr);
			TInfo(const std::string& type, InstanceParam ins = nullptr);
			TInfo(const std::string& type, InstanceParamFunc ins = nullptr);
			TInfo(const TInfo &t);
			~TInfo(void);
			TInfo& operator= (const TInfo &t);
			std::string _class;
			Instance _fun;
			InstanceFunc _func;
			InstanceParam _fun_param;
			InstanceParamFunc _func_param;
		};
		typedef std::unordered_map<std::string, TInfo>  FactoryMap;

		static myObjectFactory* getInstance();
		static void destroyInstance();
		cocos2d::Ref* createObject(const std::string &name,cocos2d::Ref*  var = nullptr);

		void registerType(const TInfo &t);
		void removeAll();

	protected:
		myObjectFactory(void);
		virtual ~myObjectFactory(void);
	private:
		static myObjectFactory *_sharedFactory;
		FactoryMap _typeMap;
	};

}
NS_CC_END

#endif
