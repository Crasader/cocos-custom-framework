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
namespace c2d {


	

	std::vector<string> split(std::string str, std::string separator);
	std::vector<std::string>getLines(const std::string &content);
	std::string nameWithoutExtension(std::string str);
	std::string pathWithoutExtension(std::string str);
	std::string getFileName(std::string str);
	std::string getFileExtension(std::string str);
	std::string getFolder(std::string filename);
	std::string getFolderPath(std::string filename);
	std::string replace_all(std::string& str, const std::string& old_value, const std::string& new_value);
	bool deleteStr(std::string& str, const std::string& sub);
	bool findStr(std::string str, const std::string sub);
	bool isDigit2(std::string str);//×Ö·û´®ÊÇ·ñÎª´¿Êý×Ö
	void pushbackTheStr(std::string& baseStr, std::string addStr);
	int toInt(std::string str);
	std::string toString(int val);
	std::string toString(Vec2 vec);
	std::string toString(Rect rect);
	

	std::string toLowerCase(std::string str);//
	std::string toUpperCase(std::string str);

	void writeStringAddToFile(const std::string& filename, const std::string& wr_str, bool newLine = true);

	std::string getLocalRes(std::string &filename);

} // namespace StringUtils {

// end of data_structure group
/// @}
NS_CC_END
#endif /* defined(__MYUTILS_H__) */
