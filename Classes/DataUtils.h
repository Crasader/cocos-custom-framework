#ifndef __DataUtils__H__
#define __DataUtils__H__
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"


USING_NS_CC;
using namespace rapidjson; 
using namespace std;

class DataUtils 
{
public:

	static DataUtils* getInstance();
	static void destroyInstance();

	bool init();
	bool readDataFromFile(const char *filename);  
	cocos2d::Value analyconfigData(const rapidjson::Value& jsonvalue);
	ValueMap getTableDatabyName(string filename);
	ValueMap getConfigData();

protected:
	~DataUtils();
private:
	DataUtils();

	DataUtils(const DataUtils &rDataUtil) = delete;

	DataUtils &operator=(const DataUtils &rDataUtil) = delete;

private:
	static DataUtils* _datamamager;

	ValueMap m_nConfigdata;//所有配置表数据

};
#endif /* defined(__DataUtils__H__) */
