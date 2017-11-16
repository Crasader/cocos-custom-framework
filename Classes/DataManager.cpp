#include "DataManager.h"

static DataManager *_datamamager;
DataManager * DataManager::shareDataManager()
{
	if (!_datamamager)
	{
		_datamamager = new DataManager();
		_datamamager->init();
	}
	return _datamamager;
}

DataManager* DataManager::getInstance()
{
	return shareDataManager();
}

bool DataManager::init()
{
	if (DataLoader::init()){
		return true;
	}
	return false;
}

void DataManager::readDataEnd(const char *filename)
{
	
}

ValueVector* DataManager::getTableDataAsValueVectorByName(const std::string& filename)
{
	CCLOG("the data -> %s", formatStr(filename).c_str());
	return &m_nConfigdata[formatStr(filename)].asValueVector();
}

ValueMap* DataManager::getTableDataAsValueMapByName(const std::string& filename)
{
	return &m_nConfigdata[formatStr(filename)].asValueMap();
}

void DataManager::initConfig()
{

}

ValueMap* DataManager::getTextDescMapByKey(const std::string& key)
{
// 	ValueVector*  nlanguagedata = getTableDataAsValueVectorByName(json_Language);
// 	auto numstr = key.substr(4);
// 	int index = std::atoi(numstr.c_str());
// 	if (index > 0 && index <= nlanguagedata->size())
// 	{
// 		auto& ndatamap = nlanguagedata->at(index - 1).asValueMap();
// 		return &ndatamap;
// 	}
	return nullptr;
}

cocos2d::Value DataManager::getValue(const cocos2d::ValueMap& setData, int rId)
{
	auto str = StringUtils::format("%d", rId);
	return setData.at(str);
}

bool DataManager::hasKey(const ValueMap& map, const std::string &key)
{
	bool has = map.find(key) != map.end();
	return has;
}

DataManager::~DataManager()
{
	
}
