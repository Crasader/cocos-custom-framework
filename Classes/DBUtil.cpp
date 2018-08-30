#include "DBUtil.h"
#include "CommonMethod.h"

sqlite3 *pDB = NULL;//数据库指针  
char * errMsg = NULL;//错误信息  
std::string sqlstr;//SQL指令  
int result;//sqlite3_exec返回值  

//创建数据库  
void DBUtil::initDB(const char *db)
{
	//串行方式打开一个数据库，如果该数据库不存在，则创建一个数据库文件  
	result = sqlite3_open_v2(db, &pDB, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX, NULL);
	if (result != SQLITE_OK)
	{
		CCLOG("打开数据库失败，错误码:%d ，错误原因:%s\n", result, errMsg);
	}
	else
	{
		sqlite3_busy_timeout(pDB, 5000);
	}
}

//tableIsExist的回调函数  
int isExisted(void * para, int n_column, char ** column_value, char ** column_name)
{
	bool *isExisted_ = (bool*)para;
	*isExisted_ = (**column_value) != '0';
	return 0;
}

//判断表格是否存在  
bool DBUtil::tableIsExist(std::string name)
{
	if (pDB != NULL)
	{
		//判断表是否存在  
		bool tableIsExisted;
		sqlstr = "select count(type) from sqlite_master where type='table' and name ='" + name + "'";
		result = sqlite3_exec(pDB, sqlstr.c_str(), isExisted, &tableIsExisted, &errMsg);
		return tableIsExisted;
	}
	return false;
}

//在数据库中判断名为name的表示否存在，如果不存在则创建这张表  
//@示例语句string sqls = "create table user(id integer,username text,password text)";  
void DBUtil::createTable(std::string sql, std::string name)
{
	if (!tableIsExist(name))
	{
		//创建表，设置ID为主键，且自动增加  
		//auto sqlstr = StringUtils::format(sql.c_str(), name.c_str());
		result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
		if (result != SQLITE_OK)
		{
			string nstr = StringUtils::format("create table failed ,error code = %d error Reason = %s", result, errMsg);
			CommonMethod::ShowWavePrompt(nstr.c_str());
		}
	}

}

//删除表格  
//@示例语句sqlstr="drop table name";  
void DBUtil::deleteTable(std::string sql, std::string name)
{
	if (tableIsExist(name))
	{
		result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
		if (result != SQLITE_OK)
		{
			string nstr = StringUtils::format("delete table failed ,error code = %d error Reason = %s", result, errMsg);
			CommonMethod::ShowWavePrompt(nstr.c_str());
		}
	}
}

//插入数据  
//@示例语句sqlstr=" insert into MyTable_1( name ) values ( '擎天柱' ) ";  
void DBUtil::insertData(std::string sql){
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
	{
		string nstr = StringUtils::format("insert table failed ,error code = %d error Reason = %s", result, errMsg);
		CommonMethod::ShowWavePrompt(nstr.c_str());
	}
}

void DBUtil::insertData(std::string tableName, std::string sql, std::string key, std::string value)
{
	std::string ncondition = StringUtils::format("where %s =%s", value.c_str());
	int itemcount = DBUtil::getDataCountByCondition(tableName, ncondition);
	if (itemcount == 0){
		result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
		if (result != SQLITE_OK)
		{
			string nstr = StringUtils::format("insert table failed ,error code = %d error Reason = %s", result, errMsg);
			CommonMethod::ShowWavePrompt(nstr.c_str());
		}
	}
	else{
		CommonMethod::ShowWavePrompt(StringUtils::format("the [ %s,%s ] data exist", key.c_str(), value.c_str()).c_str());
	}
}

void DBUtil::insertDatabatch(string sql, ValueVector datavec)
{
	result = sqlite3_exec(pDB, "begin;", NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
	{
		string nstr = StringUtils::format("insertDatabatch begin failed ,error code = %d error Reason = %s", result, errMsg);
		CommonMethod::ShowWavePrompt(nstr.c_str());
	}
	CCLOG("insertDatabatch %s", sql.c_str());
	sqlite3_stmt *stmt;
	//const char* sql = "insert into SqliteTest values(?,?,?,?)";
	sqlite3_prepare(pDB, sql.c_str(), sql.size(), &stmt, 0);
	for (int i = 0; i < datavec.size(); i++)
	{
		auto& nitemvec = datavec.at(i).asValueVector();
		sqlite3_reset(stmt);
		for (int j = 0; j < nitemvec.size(); j++)
		{
			cocos2d::Value nvalue = nitemvec.at(j);
			cocos2d::Value::Type ntype = nitemvec.at(j).getType();
			switch (ntype)
			{
			case cocos2d::Value::Type::INTEGER:
				sqlite3_bind_int(stmt, j + 1, nvalue.asInt());
				break;
			case cocos2d::Value::Type::FLOAT:
			case cocos2d::Value::Type::DOUBLE:
				sqlite3_bind_double(stmt, j + 1, nvalue.asDouble());
				break;
			case cocos2d::Value::Type::STRING:
				//sqlite3_bind_text(stmt, j + 1, nvalue.asString().c_str(), nvalue.asString().size(), SQLITE_STATIC);
				sqlite3_bind_text(stmt, j + 1, nvalue.asString().c_str(), -1, SQLITE_TRANSIENT);
				
				break;
			}
		}
		sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);
	sqlite3_exec(pDB, "commit;", NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
	{
		string nstr = StringUtils::format("insertDatabatch commit failed ,error code = %d error Reason = %s", result, errMsg);
		CommonMethod::ShowWavePrompt(nstr.c_str());
	}
}
//@示例语句sqlstr = "alter TABLE result add col integer DEFAULT 0";
//col 是列名
//DEFAULT 0 是设置默认值
void DBUtil::insertColData(std::string sql)
{
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
	{
		string nstr = StringUtils::format("Col insert table failed ,error code = %d error Reason = %s", result, errMsg);
		CommonMethod::ShowWavePrompt(nstr.c_str());
	}
}
//检索删除无用数据

void DBUtil::checkData(std::string name, std::string strkey, CheckCallBack1 callback)
{
	sqlite3_stmt *stmt = NULL;
	//char sql[200];
	string sqlstr = StringUtils::format("select * from %s", name.c_str());
	//sprintf(sql, "SELECT * FROM %s limit 0,1", name.c_str());
	char **dbResult = NULL;
	char *errmsg = NULL;
	int nRow = 0, nColumn = 0;

	//第一行是列名称  
	sqlite3_get_table((sqlite3*)pDB, sqlstr.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
	int key1index = -1;
	for (int j = 0; j < nColumn; j++)
	{
		char *pv = *(dbResult + j);
		if (strkey == pv)
		{
			key1index = j;
		}
	
	}
	if (key1index == -1)
	{
		return;
	}

	for (int i = 0; i < nRow; i++)//跳过第一行
	{
		int nnumkey1 = std::atoi(dbResult[key1index + nColumn + i*nColumn]);
		//CCLOG("nnumkey1 = %d   nnumkey2 = %d", nnumkey1, nnumkey2);
		bool check = callback(nnumkey1);
		if (!check)
		{
			string ndelsql = StringUtils::format("delete from %s where %s = %d", name.c_str(), strkey.c_str(), nnumkey1);
			deleteData(ndelsql);
		}
	}

	if (errmsg != NULL)
	{
		sqlite3_free(errmsg);
	}
	sqlite3_free_table(dbResult);
}
//检索删除无用数据
void DBUtil::checkData(std::string name, std::string strkey1, std::string strkey2, BoolCheckCallBack2 callback)
{
	sqlite3_stmt *stmt = NULL;
	//char sql[200];
	string sqlstr = StringUtils::format("select * from %s", name.c_str());
	//sprintf(sql, "SELECT * FROM %s limit 0,1", name.c_str());
	char **dbResult = NULL;
	char *errmsg = NULL;
	int nRow = 0, nColumn = 0;

	//第一行是列名称  
	sqlite3_get_table((sqlite3*)pDB, sqlstr.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
	int key1index = -1;
	int key2index = -1;
	for (int j = 0; j < nColumn; j++)
	{
		char *pv = *(dbResult + j);
		if (strkey1 == pv)
		{
			key1index = j;
		}
		if (strkey2 == pv)
		{
			key2index = j;
		}
	}
	if (key1index == -1 || key2index == -1)
	{
		return;
	}

	for (int i = 0; i < nRow; i++)//跳过第一行
	{
		int nnumkey1 = std::atoi(dbResult[key1index + nColumn + i*nColumn]);
		int nnumkey2 = std::atoi(dbResult[key2index + nColumn + i*nColumn]);
		//CCLOG("nnumkey1 = %d   nnumkey2 = %d", nnumkey1, nnumkey2);
		bool check = callback(nnumkey1, nnumkey2);
		if (!check)
		{
			string ndelsql = StringUtils::format("delete from %s where %s = %d and %s = %d", name.c_str(), strkey1.c_str(), nnumkey1, strkey2.c_str(), nnumkey2);
			deleteData(ndelsql);
		}
	}

	if (errmsg != NULL)
	{
		sqlite3_free(errmsg);
	}
	sqlite3_free_table(dbResult);
}


void DBUtil::checkData(std::string name, std::string strkey1, std::string strkey2, CheckCallBack2 callback)
{
	sqlite3_stmt *stmt = NULL;
	//char sql[200];
	string sqlstr = StringUtils::format("select * from %s", name.c_str());
	//sprintf(sql, "SELECT * FROM %s limit 0,1", name.c_str());
	char **dbResult = NULL;
	char *errmsg = NULL;
	int nRow = 0, nColumn = 0;

	//第一行是列名称  
	sqlite3_get_table((sqlite3*)pDB, sqlstr.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
	int key1index = -1;
	int key2index = -1;
	for (int j = 0; j < nColumn; j++)
	{
		char *pv = *(dbResult + j);
		if (strkey1 == pv)
		{
			key1index = j;
		}
		if (strkey2 == pv)
		{
			key2index = j;
		}
	}
	if (key1index == -1 || key2index==-1)
	{
		return;
	}

	for (int i = 0; i < nRow; i++)//跳过第一行
	{
		int nnumkey1 = std::atoi(dbResult[key1index + nColumn + i*nColumn]);
		int nnumkey2 = std::atoi(dbResult[key2index + nColumn + i*nColumn]);
		//CCLOG("nnumkey1 = %d   nnumkey2 = %d", nnumkey1, nnumkey2);
		ValueMap *nmap = callback(nnumkey1, nnumkey2);
		if (!nmap)
		{
			string ndelsql = StringUtils::format("delete from %s where %s = %d and %s = %d", name.c_str(), strkey1.c_str(), nnumkey1, strkey2.c_str(), nnumkey2);
			deleteData(ndelsql);
		}
	}

	if (errmsg != NULL)
	{
		sqlite3_free(errmsg);
	}
	sqlite3_free_table(dbResult);
}
void DBUtil::checkData(std::string name, std::string strkey1, std::string strkey2, VecCheckCallBack2 callback)
{
	sqlite3_stmt *stmt = NULL;
	//char sql[200];
	string sqlstr = StringUtils::format("select * from %s", name.c_str());
	//sprintf(sql, "SELECT * FROM %s limit 0,1", name.c_str());
	char **dbResult = NULL;
	char *errmsg = NULL;
	int nRow = 0, nColumn = 0;

	//第一行是列名称  
	sqlite3_get_table((sqlite3*)pDB, sqlstr.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
	int key1index = -1;
	int key2index = -1;
	for (int j = 0; j < nColumn; j++)
	{
		char *pv = *(dbResult + j);
		if (strkey1 == pv)
		{
			key1index = j;
		}
		if (strkey2 == pv)
		{
			key2index = j;
		}
	}
	if (key1index == -1 || key2index == -1)
	{
		return;
	}

	for (int i = 0; i < nRow; i++)//跳过第一行
	{
		int nnumkey1 = std::atoi(dbResult[key1index + nColumn + i*nColumn]);
		int nnumkey2 = std::atoi(dbResult[key2index + nColumn + i*nColumn]);
		CCLOG("nnumkey1 = %d   nnumkey2 = %d", nnumkey1, nnumkey2);
		ValueVector *nvec = callback(nnumkey1, nnumkey2);
		if (!nvec)
		{
			string ndelsql = StringUtils::format("delete from %s where %s = %d and %s = %d", name.c_str(), strkey1.c_str(), nnumkey1, strkey2.c_str(), nnumkey2);
			deleteData(ndelsql);
		}
	}

	if (errmsg != NULL)
	{
		sqlite3_free(errmsg);
	}
	sqlite3_free_table(dbResult);
}
void DBUtil::checkData(std::string name, std::string strkey1, std::string strkey2, std::string strkey3, CheckCallBack3 callback)
{
	sqlite3_stmt *stmt = NULL;
	//char sql[200];
	string sqlstr = StringUtils::format("select * from %s", name.c_str());
	//sprintf(sql, "SELECT * FROM %s limit 0,1", name.c_str());
	char **dbResult = NULL;
	char *errmsg = NULL;
	int nRow = 0, nColumn = 0;

	//第一行是列名称  
	sqlite3_get_table((sqlite3*)pDB, sqlstr.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
	int key1index = -1;
	int key2index = -1;
	int key3index = -1;
	for (int j = 0; j < nColumn; j++)
	{
		char *pv = *(dbResult + j);
		if (strkey1 == pv)
		{
			key1index = j;
		}
		if (strkey2 == pv)
		{
			key2index = j;
		}
		if (strkey3 == pv)
		{
			key3index = j;
		}
	}
	if (key1index == -1 || key2index == -1 || key3index == -1)
	{
		return;
	}

	for (int i = 0; i < nRow; i++)//跳过第一行
	{
		int nnumkey1 = std::atoi(dbResult[key1index + nColumn + i*nColumn]);
		int nnumkey2 = std::atoi(dbResult[key2index + nColumn + i*nColumn]);
		int nnumkey3 = std::atoi(dbResult[key3index + nColumn + i*nColumn]);

		CCLOG("nnumkey1 = %d   nnumkey2 = %d", nnumkey1, nnumkey2);
		ValueMap *nmap = callback(nnumkey1, nnumkey2, nnumkey3);
		if (!nmap)
		{
			string ndelsql = StringUtils::format("delete from %s where %s = %d and %s = %d", name.c_str(), strkey1.c_str(), nnumkey1, strkey2.c_str(), nnumkey2);
			deleteData(ndelsql);
		}
	}

	if (errmsg != NULL)
	{
		sqlite3_free(errmsg);
	}
	sqlite3_free_table(dbResult);
}

//删除数据  
//@示例语句sqlstr="delete from MyTable_1 where ID = 2";  
void DBUtil::deleteData(std::string sql)
{
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
	{
		string nstr = StringUtils::format("delete Data table Data failed ,error code = %d error Reason = %s", result, errMsg);
		CommonMethod::ShowWavePrompt(nstr.c_str());
	}
}
void DBUtil::clearData(std::string name)
{
	string sqlstr = StringUtils::format("delete from %s;", name.c_str());
	result = sqlite3_exec(pDB, sqlstr.c_str(), NULL, NULL, &errMsg);
	string resetsqlstr = StringUtils::format("update sqlite_sequence set seq = 0 where name = \'%s\';", name.c_str());
	result = sqlite3_exec(pDB, resetsqlstr.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
	{
		string nstr = StringUtils::format("clear table Data failed ,error code = %d error Reason = %s", result, errMsg);
		CommonMethod::ShowWavePrompt(nstr.c_str());
	}
}
//修改数据  
//@示例语句        sqlstr="update MyTable_1 set name='威震天' where ID = 3";  
void DBUtil::updateData(std::string sql)
{
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
	{
		string nstr = StringUtils::format("updateData table Data failed ,error code = %d error Reason = %s", result, errMsg);
		CommonMethod::ShowWavePrompt(nstr.c_str());
		CCLOG("%s", nstr.c_str());
	}
	CCLOG("%s", sql.c_str());
}

//getDataCount的回调函数  
int loadRecordCount(void * para, int n_column, char ** column_value, char ** column_name)
{
	int *count = (int*)para;
	*count = n_column;
	return 0;
}

//获取记录的条数  
//@示例语句string sqlsssss = "select count(*) from user";  
//@示例语句  取得表格字段的语句string sqlsssss = "select * from user";  
int DBUtil::getDataCount(std::string name)
{
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;
	string sqlstr = StringUtils::format("select * from %s", name.c_str());
	result = sqlite3_get_table(pDB, sqlstr.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
	sqlite3_free_table(dbResult);
	return nRow;

	/*int count = 0;
	string sqlstr = StringUtils::format("select count(*) from %s;", name.c_str());
	sqlite3_exec(pDB, sqlstr.c_str(), loadRecordCount, &count, &errMsg);*/
	
	//return count;
}
//例"select * from %s where %s = %d"
int DBUtil::getDataCountByCondition(std::string name, std::string conditionstr)
{
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;
	string sqlstr = StringUtils::format("select * from %s %s", name.c_str(), conditionstr.c_str());
	result = sqlite3_get_table(pDB, sqlstr.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
	sqlite3_free_table(dbResult);
	return nRow;

}
//getDataInfo的回调函数  
int loadRecord(void * para, int n_column, char ** column_value, char ** column_name)
{
	CCLOG("n_column:%d", n_column);

	//    TestVO* testVO = (TestVO*)para;  

	//   testVO->mId = atoi(column_value[0]);  
	//   testVO->level = atoi(column_value[1]);  
	//    testVO->lastscore = atoi(column_value[2]);  
	//  testVO->bestscore = atoi(column_value[3]);  
	//  testVO->star = atoi(column_value[4]);  


	/* 可能有5个字段 */
	// id level lastscore bestscore star  
	//    CCLOG("c[0]:%s,c[1]:%s,c[2]:%s,c[3]:%s,c[4]:%s",column_name[0],column_name[1],column_name[2],column_name[3],column_name[4]);  
	//      
	//CCLOG("id=%s,level=%s,lastscore=%s,bestscore=%s,star=%s", column_value[0], column_value[1], column_value[2], column_value[3], column_value[4]);
	return 0;
}
//获取一条记录的信息 其中的pSend是一个实体类我们以后可以自定义一个继承了CCObject的类来代替他保存数据库中取出来的数据  
/*
*  这里最好扩展下，让  pSend  是一个vector
*/
//void DBUtil::getDataInfo(string name, string sql, void *pSend)
//{
//	sqlite3_exec(pDB, sql.c_str(), loadRecord, pSend, &errMsg);
//}
void DBUtil::GetColName(std::string name, vector<string>& vecColName)
{
	sqlite3_stmt *stmt = NULL;
	char sql[200];
	sprintf(sql, "SELECT * FROM %s limit 0,1", name.c_str());
	char **dbResult = NULL;
	char *errmsg = NULL;
	int nRow = 0, nColumn = 0;
	//第一行是列名称  
	sqlite3_get_table((sqlite3*)pDB, sql, &dbResult, &nRow, &nColumn, &errmsg);
	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nColumn; j++)
		{
			char *pv = *(dbResult + nColumn*i + j);
			vecColName.push_back(pv);
		}
		break;
	}
	if (errmsg != NULL)
	{
		sqlite3_free(errmsg);
	}
	sqlite3_free_table(dbResult);
}

void DBUtil::getDataByCondition(std::string name, std::string conditionstr, ValueVector &vecdata)
{

	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;
	int index;
	string sqlstr = StringUtils::format("select * from %s %s", name.c_str(), conditionstr.c_str());
	result = sqlite3_get_table(pDB, sqlstr.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

	if (SQLITE_OK == result)
	{
		//查询成功
		index = nColumn;
		//CCLOG("table %s score Num =  %d", name.c_str(),nRow);
		for (int i = 0; i < nRow; i++)
		{
			//CCLOG("row index = %d ", i + 1);
			ValueMap mapdata;
			for (int j = 0; j < nColumn; j++)
			{		
				//CCLOG("field name = %s ~~~ field Value = %s", dbResult[j], dbResult[index]);
				mapdata.insert(map<std::string, cocos2d::Value>::value_type(dbResult[j], cocos2d::Value(dbResult[index])));
				++index;
			}
			cocos2d::Value dataitem(mapdata);
			vecdata.push_back(dataitem);
		}
	}

	//CCLOG("$$query %s Finish \n", name.c_str());
	sqlite3_free_table(dbResult);

}

void DBUtil::getDataInfo(std::string name, ValueVector &vecdata)
{
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;
	int index;
	string sqlstr = StringUtils::format("select * from %s", name.c_str());
	result = sqlite3_get_table(pDB, sqlstr.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
	
	vector<int> typevec;
	getDataTypeInfo(typevec, name);

	if (SQLITE_OK == result)
	{
		//查询成功
		index = nColumn; 
		//CCLOG("table %s score Num =  %d", name.c_str(),nRow);
		for (int i = 0; i < nRow; i++)
		{
			//CCLOG("row index = %d ", i + 1);
			ValueMap mapdata;
			for (int j = 0; j < nColumn; j++)
			{
				//CCLOG("typevec%d = %d",j, typevec[j]);
				switch (typevec[j])
				{
				case 1:
					//SQLITE_INTEGER  
					mapdata.insert(map<std::string, cocos2d::Value>::value_type(dbResult[j], cocos2d::Value(atoi(dbResult[index]))));
					break;
				case 2:
					//SQLITE_FLOAT  
					mapdata.insert(map<std::string, cocos2d::Value>::value_type(dbResult[j], cocos2d::Value(atof(dbResult[index]))));
					break;
				case 3:
					//SQLITE_TEXT  
					mapdata.insert(map<std::string, cocos2d::Value>::value_type(dbResult[j], cocos2d::Value(dbResult[index])));
					break;
				case 4:
					//SQLITE_BLOB 
					//暂不支持二进制存储
					break;
				case 5:
					//SQLITE_NULL  
					//暂不支持空存储
					break;
				}
				//CCLOG("field name = %s ~~~ field Value = %s", dbResult[j], dbResult[index]);
				//mapdata.insert(map<std::string, cocos2d::Value>::value_type(dbResult[j], cocos2d::Value(dbResult[index])));
				++index; 
			}
			cocos2d::Value dataitem(mapdata);
			vecdata.push_back(dataitem);
		}
	}

	//CCLOG("$$query %s Finish \n", name.c_str());
	sqlite3_free_table(dbResult);
	
}
void DBUtil::getDataTypeInfo(std::vector<int>& vecType, std::string name)
{
	sqlite3_stmt *stmt = NULL;
	char sql[200];
	sprintf(sql, "SELECT * FROM %s limit 0,1", name.c_str());
	sqlite3_prepare((sqlite3*)pDB, sql, -1, &stmt, 0);
	if (stmt)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			int nCount = sqlite3_column_count(stmt);
			for (int i = 0; i < nCount; i++)
			{
				int nValue = sqlite3_column_int(stmt, 0);
				int nType = sqlite3_column_type(stmt, i);
				vecType.push_back(nType);
				switch (nType)
				{
				case 1:
					//SQLITE_INTEGER  
					break;
				case 2:
					//SQLITE_FLOAT  
					break;
				case 3:
					//SQLITE_TEXT  
					break;
				case 4:
					//SQLITE_BLOB  
					break;
				case 5:
					//SQLITE_NULL  
					break;
				}
			}
			break;
		}
		sqlite3_finalize(stmt);
		stmt = NULL;
	}
}
//例 "select Age from TableName where Name = LiLei"
cocos2d::Value DBUtil::getOneData(std::string sql)
{
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;
	int index;
	char* rResult;
	result = sqlite3_get_table(pDB, sql.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

	CCLOG("getOneData row is %d,column is %d", nRow, nColumn);
	if (SQLITE_OK == result)
	{
		//key = value
		auto key = dbResult[0];// key 
		rResult = dbResult[1];// value
	}
	else{
		rResult = "";
		string nstr = StringUtils::format("getOneData failed ,error code = %d error Reason = %s", result, errMsg);
		CommonMethod::ShowWavePrompt(nstr.c_str());
	}

	cocos2d::Value va(rResult);
	sqlite3_free_table(dbResult);

	return va;
}

//关闭数据库  
void DBUtil::closeDB()
{
	sqlite3_close(pDB);
}
void DBUtil::TestSqlData(std::string sql)
{
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
		CCLOG("插入记录失败，错误码:%d ，错误原因:%s\n", result, errMsg);
}

bool DBUtil::emptyTable(std::string tableName)
{
	return getDataCount(tableName) == 0;
}
