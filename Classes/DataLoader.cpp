//
//  DataLoader.cpp

#include "DataLoader.h"




DataLoader* DataLoader::_pDataLoader;


DataLoader::DataLoader()
{

}

DataLoader::~DataLoader()
{
	m_nConfigdata.clear();
}

DataLoader * DataLoader::getInstance()
{
	if (!_pDataLoader)
	{
		_pDataLoader = new DataLoader();
		_pDataLoader->init();
	}
	return _pDataLoader;
}

void DataLoader::destroyInstance()
{
	if (_pDataLoader)CC_SAFE_DELETE(_pDataLoader);
}

bool DataLoader::init()
{
	return true;
}
bool DataLoader::readDataFromFile(const char *filename)
{
	rapidjson::Document readdoc;

	std::string temp = formatStr(filename);
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(temp);
 	readdoc.Parse<0>(contentStr.c_str());
	if (readdoc.HasParseError())
	{
		CCLOG("GetParseError%u\n", readdoc.GetParseError());
		return false;
	}

	if (!readdoc.IsNull()){
		cocos2d::Value nvalue = analyconfigData(readdoc);
		auto item = map<std::string, cocos2d::Value>::value_type(temp.c_str(), nvalue);
		m_nConfigdata.insert(item);
	}
	
	return true;
}

bool DataLoader::readDataFromFileOtherThread(const char *filename, std::mutex* _mutex)
{
	rapidjson::Document readdoc;

	std::string temp = formatStr(filename);
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(temp);
	readdoc.Parse<0>(contentStr.c_str());
	if (readdoc.HasParseError())
	{
		CCLOG("GetParseError%u\n", readdoc.GetParseError());
		return false;
	}

	if (!readdoc.IsNull()){
		cocos2d::Value nvalue = analyconfigData(readdoc);
		auto item = map<std::string, cocos2d::Value>::value_type(temp.c_str(), nvalue);
		_mutex->lock();
		m_nConfigdata.insert(item);
		_mutex->unlock();
	}

	return true;
}

cocos2d::ValueMap DataLoader::readDataFromStr(const std::string& contentStr)
{
	cocos2d::ValueMap rValueMap;
	rapidjson::Document readdoc;
	readdoc.Parse<0>(contentStr.c_str());
	if (readdoc.HasParseError())
	{
		CCLOG("GetParseError%u\n", readdoc.GetParseError());
		return rValueMap;
	}

	if (!readdoc.IsNull()){
		cocos2d::Value nvalue = analyconfigData(readdoc);
		rValueMap = nvalue.asValueMap();
	}

	return rValueMap;
}

cocos2d::Value DataLoader::analyconfigData(const rapidjson::Value& jsonvalue)
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
			//CCLOG("the key is ---> %s", key.c_str());
			const rapidjson::Value& subvalue = jsonvalue[key.c_str()];
			cocos2d::Value nanalyvalue = analyconfigData(subvalue);
			nmap.insert(map<std::string, cocos2d::Value>::value_type(key, nanalyvalue));
		}

		item = nmap;

	}
	if (jsonvalue.IsString())
	{
		//CCLOG("the string is -- >  %s", jsonvalue.GetString());
		item = jsonvalue.GetString();
	}
	if (jsonvalue.IsInt())
	{
		//CCLOG("the GetInt is -- >  %ld", jsonvalue.IsInt());
		item = jsonvalue.GetInt();
	}
	if (jsonvalue.IsDouble())
	{
		//CCLOG("the GetDouble is -- >  %d", jsonvalue.GetDouble());
		item = jsonvalue.GetDouble();
	}
	
	return item;
}

cocos2d::Value* DataLoader::getTableDataByName(const string& filename)
{
	CCLOG("the getTableDataByName address -> %x", &m_nConfigdata[formatStr(filename)]);
	return &m_nConfigdata[formatStr(filename)];
}

const cocos2d::ValueMap& DataLoader::getConfigData()
{
	return m_nConfigdata;
}

std::string DataLoader::formatStr(const string &str)
{
	std::string temp = str;
	if (temp.find(".json") < temp.length()){
		//包含json字段
		temp = str;
	}
	else{
		temp = StringUtils::format("%s.json", str.c_str());
	}
	return temp;
}
