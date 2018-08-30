#pragma once

#include "i18n.h"
#include "cocos2d.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include "json/document.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d::ui;
using namespace std;
using namespace rapidjson;

USING_NS_CC;

static Document* instance = nullptr;
static Document* typeinstance = nullptr;

static std::string _sLanguage = "English";

static int _iLanguageIndex = 1;

static std::string _sLanguageCode = "en";

static std::string _sCustomLanguageCode = "en";


static std::map<std::string, float> _cacheFontSize;

static std::map<std::string, std::map<std::string, float>> _scale;/*= {
	{ "English", { { "des", 1.0f }, { "nvg", 1.0f }, { "btn", 1.0f } } },
	{ "Chinese", { { "des", 0.857f }, { "nvg", 0.878f }, { "btn", 0.878f } } },
	};*/
void i18n::load()
{
	if (!instance) {
		init();
		{
			_scale["English"]["des"] = 1.0f;
			_scale["English"]["nvg"] = 1.0f;
			_scale["English"]["btn"] = 1.0f;
			_scale["English"]["zzgfdh"] = 1.0f;

			_scale["Chinese"]["des"] = 0.857f;
			_scale["Chinese"]["nvg"] = 0.878f;
			_scale["Chinese"]["btn"] = 0.878f;
			_scale["Chinese"]["zzgfdh"] = 0.888f;
		}
		{
			auto fileName = StringUtils::format("res/i18n/%s.json", _sLanguage.c_str());
			auto fileUtils = FileUtils::getInstance();
			CC_ASSERT(fileUtils->isFileExist(fileName));

			auto string = fileUtils->getStringFromFile(fileName);
			instance = new Document;
			instance->Parse(string.c_str());
		}



		{
			auto fileName = StringUtils::format("res/i18n/Type.json", _sLanguage.c_str());
			auto fileUtils = FileUtils::getInstance();
			CC_ASSERT(fileUtils->isFileExist(fileName));

			auto string = fileUtils->getStringFromFile(fileName);
			typeinstance = new Document;
			typeinstance->Parse(string.c_str());
			
		}
	}
}

void i18n::reload()
{
	delete instance;
	instance = nullptr;
	load();
}

std::string i18n::getType(const std::string& key)
{
	if (!typeinstance){
		i18n::load();
	}
	if (typeinstance->HasMember(key)) {
		return (*typeinstance)[key].GetString();
	}

	return "des";
}

std::string i18n::getTTF(const std::string& key)
{
	std::string type = getType(key);
	std::string str = "";
	if (type == "des"){
		str = "ttf";
	}
	else if (type == "nvg"){
		str = "ttfbd";
	}
	else if (type == "btn"){
		str = "ttfbd";
	}
	else if (type == "zzgfdh"){
		if (_sLanguage == "Chinese"){
			str = "zzgfdh";
		}
	}

	if (str == ""){
		return "";
	}

	auto fileName = StringUtils::format("AllRes/Fonts/%s/%s.ttf", _sLanguage.c_str(), str.c_str());
	return fileName;
}

std::string i18n::getString(const std::string& key)
{	
	if (!instance) {
		load();
	}
	if (instance->HasMember(key)) {
		auto str = (*instance)[key].GetString();
		CCLOG("str: %s", str);
		return str;
	}
	else {
		CCLOG("%s not found in i18n files", key.c_str());

		return key;
	}
	return "";
}

void i18n::setTextString(Widget* label, std::string key,...)
{
	std::string nstrdescri = getString(key);
	std::string ttf = getTTF(key);
	std::string type = getType(key);
	float fontscale = _scale[_sLanguage][type];
	auto text = static_cast<Text*>(label);
	float oldssize = text->getFontSize();
	auto name = text->getName();
	auto pos = text->getPosition();
	auto uid = StringUtils::format("%s%.02f%.02f", name.c_str(), pos.x, pos.y);
	auto it = _cacheFontSize.find(uid);
	if (it != _cacheFontSize.end()){
		oldssize = it->second;
	}
	else{
		_cacheFontSize.insert(std::make_pair(uid, oldssize));
	}

	if (ttf != ""){
		text->setFontName(ttf);
	}
	
	text->setFontSize(oldssize * fontscale);
#define CC_MAX_STRING_LENGTH (1024*100)

	std::string ret;
	const char* format = nstrdescri.c_str();
	va_list ap;
	va_start(ap, key);

	char* buf = (char*)malloc(CC_MAX_STRING_LENGTH);
	if (buf != nullptr)
	{
		vsnprintf(buf, CC_MAX_STRING_LENGTH, format, ap);
		ret = buf;
		free(buf);
	}
	va_end(ap);

	text->setString(ret);
}

void i18n::setTextStringNoArgs(Widget* label, std::string key)
{

	std::string nstrdescri = getString(key);
	std::string ttf = getTTF(key);
	std::string type = getType(key);
	float fontscale = _scale[_sLanguage][type];
	auto text = static_cast<Text*>(label);
	float oldssize = text->getFontSize();
	auto name = text->getName();
	auto pos = text->getPosition();
	auto uid = StringUtils::format("%s", name.c_str());
	auto it = _cacheFontSize.find(uid);
	if (it != _cacheFontSize.end()){
		oldssize = it->second;
	}
	else{
		_cacheFontSize.insert(std::make_pair(uid, oldssize));
	}

	if (ttf != ""){
		text->setFontName(ttf);
	}
	text->setFontSize(oldssize * fontscale);

	text->setString(nstrdescri);
}

void i18n::init()
{
	std::string nlanguage = "English";
	int nlanguageindex = 2;
	LanguageType currentLanguageType = Application::getInstance()->getCurrentLanguage();

	switch (currentLanguageType)
	{
		//英文	印度尼西亚	德语	韩语	俄语	法语	葡萄牙语	意大利语	西班牙语
	case LanguageType::ENGLISH:
	{
								  FileUtils::getInstance()->addSearchPath("res/Fnt/en");
								  nlanguage = "English";
								  _sCustomLanguageCode = "en";
								  nlanguageindex = 2;
	}
		break;
	case LanguageType::CHINESE:
	{
								  FileUtils::getInstance()->addSearchPath("res/Fnt/zh");
 								  nlanguage = "Chinese";
 								  _sCustomLanguageCode = "zh";

// 								  nlanguage = "English";
// 								  _sCustomLanguageCode = "en";
								  nlanguageindex = 2;
								  // 								  int ntype = SDKManager::getChineseType();
								  // 								  if (ntype == 0)
								  // 								  {
								  // 									  FileUtils::getInstance()->addSearchPath("res/Fnt/zh");
								  // 									  nlanguage = "Chinese";
								  // 									  nlanguageindex = 0;
								  // 								  }
								  // 								  else if (ntype == 1)
								  // 								  {
								  // 									  FileUtils::getInstance()->addSearchPath("res/Fnt/tw");
								  // 									  nlanguage = "Chinese";
								  // 									  nlanguageindex = 1;
								  // 								  }
	}
		break;
	
		break;
	default:
	{
			   FileUtils::getInstance()->addSearchPath("res/Fnt/en");
			   nlanguage = "English";
	}
		break;
	}

// 	nlanguage = "Chinese";
// 	_sCustomLanguageCode = "zh";
	_sLanguageCode = Application::getInstance()->getCurrentLanguageCode();
	_sLanguage = nlanguage;
	_iLanguageIndex = nlanguageindex;
}

int i18n::getIndex()
{
	return _iLanguageIndex;
}

std::string i18n::getLanguage()
{
	return _sLanguage;
}

const char* i18n::getLanguageCode()
{
	LanguageType currentLanguageType = Application::getInstance()->getCurrentLanguage();

	return _sLanguageCode.c_str();
}

const char* i18n::getCustomLanguageCode()
{
	return _sCustomLanguageCode.c_str();
}
