//
//  DataManager.h

#ifndef __DataManager__
#define __DataManager__
#include "DataLoader.h"
class DataManager :public DataLoader
{
public:
	~DataManager();
	static DataManager* shareDataManager();
	static DataManager* getInstance();
	bool init();

	virtual void readDataEnd(const char *filename);
	virtual cocos2d::Value getValue(const cocos2d::ValueMap& setData, int id);
	virtual bool hasKey(const ValueMap& map, const std::string &key);

	//获取指定表数据接口
	ValueVector* getTableDataAsValueVectorByName(const std::string& filename);
	ValueMap* getTableDataAsValueMapByName(const std::string& filename);

	
	ValueMap* getTextDescMapByKey(const std::string& key);

	void initConfig();
private:
	
	
};
#endif /* defined(__DataManager__) */
