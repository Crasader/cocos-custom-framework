#pragma once
#include <string>
#include "i18n_variable.h"
#include "cocos2d.h"
using namespace std;
using namespace cocos2d::ui;

class i18n
{
public:
	static void load();
	static void reload();
	static void init();
	static std::string getString(const std::string& key);
	static std::string getTTF(const std::string& key);
	static std::string getType(const std::string& key);
	static void setTextString(Widget* label,std::string key,...);
	static void setTextStringNoArgs(Widget* label, std::string key);

	static int getIndex();
	static std::string getLanguage();
	static const char* getLanguageCode();
	static const char* getCustomLanguageCode();

};