#ifndef __MYUTILS_H__
#define __MYUTILS_H__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
#include <string.h>
#endif

#include "cocos2d.h"
#include <thread>
#include <vector>
#include <map>
#include <functional>
#include <mutex>
#include <stdarg.h>
#include <string>
#include <functional>
#include <sstream>
#include "base/CCRef.h"
#include <algorithm>
using namespace std;
using namespace cocos2d;
using namespace cocos2d::experimental;

NS_CC_BEGIN
namespace StringUtil {


	

	std::vector<std::string> split(const std::string & str, const std::string & separator);
	int charCount(const std::string & str, const std::string & separator);
	std::vector<std::string>getLines(const std::string &content);
	std::string nameWithoutExtension(const std::string & str);
	std::string pathWithoutExtension(const std::string & str);
	std::string getFileName(const std::string & str);
	std::string getFileExtension(const std::string & str);
	std::string getFolder(const std::string &filename);
	std::string getFolderPath(const std::string & filename);
	std::string replace_all(const std::string& str, const std::string& old_value, const std::string& new_value);
	bool deleteStr(const std::string& str, const std::string& sub);
	bool findStr(const std::string& str, const std::string& bef);


	bool isDigit2(const std::string& str);//字符串是否为纯数字
	void pushbackTheStr(std::string baseStr, const std::string& addStr);
	int toInt(const std::string& str);
	std::string toString(int val);
	std::string toString(const Vec2& vec);
	std::string toString(const Vec3& vec);
	std::string toString(const Rect& rect);

	//格式化成货币，每3位 加一个逗号： 2,100,000.00
	std::string format2currency(float val);
	std::string format2currency(int val);
	std::string format2currency(const std::string& str);

	std::string toLowerCase(const std::string& str);//
	std::string toUpperCase(const std::string& str);

	void writeStringAddToFile(const std::string& filename, const std::string& wr_str, bool newLine = true);

	//输入格式为 00:00:00 输出 为 秒
	int time2sec(const std::string& time);

	std::string timeFormat(int time, const std::string& format = "%02d:%02d:%02d");


	std::string getLocalRes(const std::string&filename);
	//读取文件中存储的文件名
	std::vector<std::string> getFileTxtFiles(const std::string& path, const std::string& filename);
	std::vector<std::string> getFileTxtFiles(const std::string& fullpath);
	/**
	* 读取配置文件配置文件格式为
	* key1=value1
	* key2=value2
	*/
	std::map<std::string, std::string> loadProperties(const std::string &fullpath);

	bool writeProperties(const std::string &fullpath, const std::map<std::string, std::string>& contentStr);
} // namespace StringUtils {

// end of data_structure group
/// @}
NS_CC_END
#endif /* defined(__MYUTILS_H__) */
