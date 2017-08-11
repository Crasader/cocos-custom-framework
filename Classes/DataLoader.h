#ifndef __DataLoader__H__
#define __DataLoader__H__
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"


USING_NS_CC;
using namespace rapidjson; 
using namespace std;

class DataLoader 
{
public:
	DataLoader();
	~DataLoader();

	static DataLoader* getInstance();
	static void destroyInstance();

	virtual bool init();
	virtual bool readDataFromFile(const char *filename);
	virtual bool readDataFromFileOtherThread(const char *filename,std::mutex* _mutex);
	virtual ValueMap readDataFromStr(const std::string& contentStr);
	virtual cocos2d::Value analyconfigData(const rapidjson::Value& jsonvalue);
	virtual cocos2d::Value* getTableDataByName(const std::string& filename);
	virtual const ValueMap& getConfigData();
	virtual std::string formatStr(const string &str);
private:

	DataLoader(const DataLoader &rDataLoader) = delete;

	DataLoader &operator=(const DataLoader &rDataLoader) = delete;

protected:
	static DataLoader* _pDataLoader;

	ValueMap m_nConfigdata;//所有配置表数据

};
#endif /* defined(__DataLoader__H__) */
