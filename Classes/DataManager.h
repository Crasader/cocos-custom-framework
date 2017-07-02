
#ifndef __DATAMANAGER_H__
#define __DATAMANAGER_H__
#include "cocos2d.h"
#include "DataUtils.h"

USING_NS_CC;
using namespace std;

class DataManager
{
public:
	static DataManager* getInstance();
	static void destroyInstance();


	virtual cocos2d::ValueMap getSetData(const int &rId, const std::string &rSFileName);
	virtual std::string getString(const int &rId, const std::string &rKey, const std::string &rSFileName);
	virtual int getInt(const int &rId, const std::string &rKey, const std::string &rSFileName);
	virtual double getFloat(const int &rId, const std::string &rKey, const std::string &rSFileName);
	virtual bool hasKey(const int &rId, const std::string &rKey, const std::string &rSFileName);


	virtual cocos2d::ValueMap getSetData(const std::string &rId, const std::string &rSFileName);
	virtual std::string getString(const std::string &rId, const std::string &rKey, const std::string &rSFileName);
	virtual int getInt(const std::string &rId, const std::string &rKey, const std::string &rSFileName);
	virtual double getFloat(const std::string &rId, const std::string &rKey, const std::string &rSFileName);

	virtual cocos2d::Value getValue(cocos2d::ValueMap setData, const std::string &key);
	
	virtual bool hasKey(const std::string &rId, const std::string &rKey, const std::string &rSFileName);

	virtual bool hasKey(ValueMap map, const std::string &key);

private:
	void init();
	static DataManager* instance;
private:
	
	DataManager();
	~DataManager();
	DataManager(const DataManager &rDataUtil) = delete;

	DataManager &operator=(const DataManager &rDataUtil) = delete;


};

#endif /* defined(__DATAMANAGER_H__) */
