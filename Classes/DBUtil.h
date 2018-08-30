#ifndef __DBUtil_H__
#define __DBUtil_H__
#include "Macro.h"
#include "sqlite3.h"
class DBUtil
{
public:
	/************************************************************
	封装 sqlite3操作
	************************************************************/
	//用来创建一个db数据库 db为数据库的名字  
	//  打开数据库  
	static void initDB(const char *db);

	//用来判断表格是否存在  
	// name:表示表名  
	static bool tableIsExist(std::string name);

	//用来创建一个表名为name的表格，创建时会先匹配时否有该表的存在如果存在则不创建  
	//创建表  
	static void createTable(std::string sql, std::string name);

	//用来删除一张表名为name的表格，删除时会先匹配是否有该表的存在如果不存在则不执行删除操作  
	//删除表名  
	static void deleteTable(std::string sql, std::string name);

	//用来向表中插入一条数据  
	//插入一条数据  
	static void insertData(std::string sql);

	//检查是否含有相同 id 的数据，如果没有则插入，如果有则跳过
	static void insertData(std::string tableName, std::string sql, std::string key, std::string value);

	//批量向数据库中插入数据
	static void insertDatabatch(string sql, ValueVector datavec);
	//用来向表中插入一列数据
	static void insertColData(std::string sql);


	//检索删除无用数据
	static void checkData(std::string name, std::string strkey, CheckCallBack1 callback);
	static void checkData(std::string name, std::string strkey1, std::string strkey2, BoolCheckCallBack2 callback);
	static void checkData(std::string name, std::string strkey1, std::string strkey2, CheckCallBack2 callback);
	static void checkData(std::string name, std::string strkey1, std::string strkey2, VecCheckCallBack2 callback);
	static void checkData(std::string name, std::string strkey1, std::string strkey2, std::string strkey3, CheckCallBack3 callback);
	//用来向表中删除一条数据  
	//删除一条数据  
	static void deleteData(std::string sql);
	static void clearData(std::string name);
	//用来向表中修改一条数据  
	// 修改一条数据  
	static void updateData(std::string sql);

	//判断是否是空的
	static bool emptyTable(std::string tableName);

	//获取一个记录的条数  
	// 获得记录的条数  
	static int getDataCount(std::string name);
	//获取指定id记录存在条数
	static int getDataCountByCondition(std::string name, std::string conditionstr);
	//获取表列名信息
	static void GetColName(std::string name, vector<std::string>& vecColName);
	//读取一条记录的信息  
	/*
	*  此方法是查询方法,相当之重要,pSender最好是个vector
	*/
	static void getDataInfo(std::string name, ValueVector &vecdata);
	static void getDataTypeInfo(std::vector<int>& vecType, std::string name);

	// 读取一条数据
	static cocos2d::Value getOneData(std::string sql);
	static void getDataByCondition(std::string name, std::string conditionstr, ValueVector &vecdata);
	//关闭打开的数据库  
	static void closeDB();

	static void TestSqlData(std::string sql);
};

#endif // __AchieveView_H__
