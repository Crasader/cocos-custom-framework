#include "DataManager.h"

DataManager* DataManager::instance = nullptr;
DataManager::DataManager()
{
}

DataManager::~DataManager()
{

}

DataManager* DataManager::getInstance()
{
	if (!instance){
		instance = new (std::nothrow) DataManager();
		instance->init();
	}
	return instance;
}

void DataManager::destroyInstance()
{
	CC_SAFE_DELETE(instance);
}

void DataManager::init()
{

}

cocos2d::ValueMap DataManager::getSetData(const int &rId, const std::string &rSFileName)
{
	string str = StringUtils::format("%d", rId);
	return getSetData(str, rSFileName);
}

cocos2d::ValueMap DataManager::getSetData(const std::string &rId, const std::string &rSFileName)
{
	cocos2d::ValueMap data = DataUtils::getInstance()->getTableDatabyName(rSFileName);
	
	bool isExist = data.find(rId.c_str()) != data.end();
	if (!isExist)return ValueMap();
	cocos2d::ValueMap setData = data.at(rId.c_str()).asValueMap();
	return setData;
}

std::string DataManager::getString(const std::string &rId, const std::string &rKey, const std::string &rSFileName)
{
	cocos2d::ValueMap setData = getSetData(rId, rSFileName);
	return  setData.at(rKey).asString();
}

int DataManager::getInt(const std::string &rId, const std::string &rKey, const std::string &rSFileName)
{
	cocos2d::ValueMap setData = getSetData(rId, rSFileName);
	return  setData.at(rKey).asInt();
}

double DataManager::getFloat(const std::string &rId, const std::string &rKey, const std::string &rSFileName)
{
	cocos2d::ValueMap setData = getSetData(rId, rSFileName);
	return  setData.at(rKey).asFloat();
}
std::string DataManager::getString(const int &rId, const std::string &rKey, const std::string &rSFileName)
{
	cocos2d::ValueMap setData = getSetData(rId, rSFileName);
	return  setData.at(rKey).asString();
}

int DataManager::getInt(const int &rId, const std::string &rKey, const std::string &rSFileName)
{

	cocos2d::ValueMap setData = getSetData(rId, rSFileName);
	return  setData.at(rKey).asInt();
}

double DataManager::getFloat(const int &rId, const std::string &rKey, const std::string &rSFileName)
{
	cocos2d::ValueMap setData = getSetData(rId, rSFileName);

	return  setData.at(rKey).asFloat();
}

bool DataManager::hasKey(const std::string &rId, const std::string &rKey, const std::string &rSFileName)
{
	cocos2d::ValueMap setData = getSetData(rId, rSFileName);
	return  hasKey(setData, rKey);
}

bool DataManager::hasKey(const int &rId, const std::string &rKey, const std::string &rSFileName)
{
	cocos2d::ValueMap setData = getSetData(rId, rSFileName);
	return  hasKey(setData, rKey);
}
bool DataManager::hasKey(ValueMap map, const std::string &key)
{
	bool has = map.find(key)!= map.end();
	return has;
}

cocos2d::Value DataManager::getValue(cocos2d::ValueMap setData, const std::string &key)
{
	if (hasKey(setData, key)){
		return setData.at(key);
	}
	return cocos2d::Value();
}
