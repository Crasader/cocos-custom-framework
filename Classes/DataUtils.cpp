//
//  DataUtils.cpp

#include "DataUtils.h"




DataUtils* DataUtils::_datamamager;


DataUtils::DataUtils()
{

}

DataUtils::~DataUtils()
{
	m_nConfigdata.clear();
}

DataUtils * DataUtils::getInstance()
{
	if (!_datamamager)
	{
		_datamamager = new DataUtils();
		_datamamager->init();
	}
	return _datamamager;
}

void DataUtils::destroyInstance()
{
	if (_datamamager)CC_SAFE_DELETE(_datamamager);
}

bool DataUtils::init()
{
	return true;
}
bool DataUtils::readDataFromFile(const char *filename)
{
	rapidjson::Document readdoc; 
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(filename);
 	readdoc.Parse<0>(contentStr.c_str());
	if (readdoc.HasParseError())
	{
		CCLOG("GetParseError%u\n", readdoc.GetParseError());
		return false;
	}

	if (!readdoc.IsNull()){
		cocos2d::Value nvalue = analyconfigData(readdoc);
		auto item = map<std::string, cocos2d::Value>::value_type(filename, nvalue);
		m_nConfigdata.insert(item);
	}
	
	return true;
}

cocos2d::Value DataUtils::analyconfigData(const rapidjson::Value& jsonvalue)
{
	cocos2d::Value item;
	if (jsonvalue.IsArray())
	{
		ValueVector nvec;
		for (int i = 0; i < jsonvalue.Size(); i++)
		{
			const rapidjson::Value& subvalue = jsonvalue[i];
			cocos2d::Value nanalyvalue = analyconfigData(subvalue);
			nvec.push_back(nanalyvalue);
		}
		
		item = nvec;
	}
	if (jsonvalue.IsObject())
	{
		ValueMap nmap;
		int size = jsonvalue.MemberCount();
		
		
		for (auto it = jsonvalue.MemberBegin(); it != jsonvalue.MemberEnd(); it++)//遍历所有的key
		{
			std::string key = StringUtils::format("%s", (*it).name.GetString());
			const rapidjson::Value& subvalue = jsonvalue[key.c_str()];
			cocos2d::Value nanalyvalue = analyconfigData(subvalue);
			nmap.insert(map<std::string, cocos2d::Value>::value_type(key, nanalyvalue));
		}

		item = nmap;

	}
	if (jsonvalue.IsString())
	{
		item = jsonvalue.GetString();
	}
	if (jsonvalue.IsInt())
	{
		item = jsonvalue.GetInt();
	}
	if (jsonvalue.IsDouble())
	{
		item = jsonvalue.GetDouble();
	}
	
	return item;
}

ValueMap DataUtils::getTableDatabyName(string filename)
{
	cocos2d::Value va = m_nConfigdata[filename];
	return va.asValueMap();
}

cocos2d::ValueMap DataUtils::getConfigData()
{
	return m_nConfigdata;
}
