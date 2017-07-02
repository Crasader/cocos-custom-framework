
#include "myStringUtils.h"

#include <stdlib.h>
#include <stdio.h>

NS_CC_BEGIN

namespace c2d {


	std::vector<string> split(std::string str, std::string separator)
	{

		std::vector<string> result;
		int cutAt;
		while ((cutAt = str.find_first_of(separator)) != str.npos)
		{
			if (cutAt > 0)
			{
				result.push_back(str.substr(0, cutAt));
			}
			str = str.substr(cutAt + 1);
		}
		if (str.length() > 0)
		{
			result.push_back(str);
		}

		return result;
	}

	std::vector<std::string> getLines(const std::string &content)
	{
		std::vector<std::string> rows;
		char rowSeperator = '\n';
		std::string::size_type lastIndex = content.find_first_not_of(rowSeperator, 0);
		std::string::size_type currentIndex = content.find_first_of(rowSeperator, lastIndex);

		while (std::string::npos != currentIndex || std::string::npos != lastIndex) {
			rows.push_back(content.substr(lastIndex, currentIndex - lastIndex - 1));
			lastIndex = content.find_first_not_of(rowSeperator, currentIndex);
			currentIndex = content.find_first_of(rowSeperator, lastIndex);
		}
		return rows;
	}
	
	std::string nameWithoutExtension(std::string str)
	{
		std::string name = str;
		name = replace_all(str, "\\", "/");

		int slashIndex = name.find_last_of('/');
		if (slashIndex != -1){
			name = name.substr(slashIndex + 1, name.length() - 1);
		}
		int dotIndex = name.find_last_of('.');
		if (dotIndex == -1) return name;
		return name.substr(0, dotIndex);
	}

	std::string pathWithoutExtension(std::string str)
	{
		std::string name = str;

		int dotIndex = str.find_last_of('.');
		if (dotIndex == -1) return name;
		return name.substr(0, dotIndex);
	}
	std::string getFileName(std::string str)
	{
		std::string name = str;
		name = replace_all(str, "\\", "/");

		int slashIndex = name.find_last_of('/');
		if (slashIndex != -1){
			name = name.substr(slashIndex + 1, name.length() - 1);
		}
		return name;
	}

	std::string getFileExtension(std::string str)
	{
		std::string fileExtension;
		size_t pos = str.find_last_of('.');
		if (pos != std::string::npos)
		{
			fileExtension = str.substr(pos, str.length());

			std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
		}

		return fileExtension;
	}

	std::string getFolder(std::string filename)
	{
		auto folderPath = getFolderPath(filename);
		auto folderName = getFileName(folderPath);
		return folderName;
	}

	std::string getFolderPath(std::string filename)
	{
		string name = filename;
		name = replace_all(filename, "\\", "/");

		int slashIndex = name.find_last_of('/');
		if (slashIndex != -1){
			name = name.substr(0, slashIndex);
		}
		return name;
	}
	std::string replace_all(std::string& str, const std::string& old_value, const std::string& new_value)
	{
		while (true)   {
			std::string::size_type   pos(0);
			if ((pos = str.find(old_value)) != string::npos)
				str.replace(pos, old_value.length(), new_value);
			else   break;
		}
		return str;
	}
	int toInt(string str)
	{
		int number = atoi(str.c_str());
		return number;
	}

	std::string toString(int val)
	{
		return cocos2d::StringUtils::format("%d", val);
	}

	std::string toString(Vec2 vec)
	{
		return cocos2d::StringUtils::format("Vec2 [%.02f,%.02f]", vec.x, vec.y);
	}

	std::string toString(Rect rect)
	{
		return cocos2d::StringUtils::format("Rect  origin [%.02f,%.02f],size [%.02f,%.02f]", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
	}

	void pushbackTheStr(std::string& baseStr, std::string addStr)
	{
		baseStr = StringUtils::format("%s %s\n", baseStr.c_str(), addStr.c_str());
	}

	void writeStringAddToFile(const std::string& filename, const std::string& wr_str, bool newLine/* = true*/)
	{
		std::string writablePath = FileUtils::getInstance()->getWritablePath();
		std::string fullPath = writablePath + filename;
		std::string str = FileUtils::getInstance()->getStringFromFile(fullPath);
		if (newLine){
			str += "\n";
		}
		str += wr_str;

		if (FileUtils::getInstance()->writeStringToFile(str, fullPath.c_str()))
		{
			log("see the log at %s", fullPath.c_str());

		}
	}

	bool deleteStr(std::string& str, const std::string& sub)
	{
		bool isDone = false;
		int n = sub.size();
		int m, flag = 0, num = 0;           //num是子串出现的次数
		while (flag == 0)
		{
			m = str.find(sub);
			if (m < 0)
				flag = 1;
			else
			{
				isDone = true;
				str.erase(m, n);           //删除子串
				num++;
			}
		}
		return isDone;
	}

	bool findStr(std::string str, const std::string item)
	{
		int m = str.find(item);
		return m >= 0 ;
	}

	std::string getLocalRes(std::string &filename)
	{
		std::string temp = filename;
		auto path = FileUtils::getInstance()->getWritablePath();
		auto fullpath = StringUtils::format("%s%s", path.c_str(), temp.c_str());
		return fullpath;
	}

	std::string toLowerCase(std::string str)
	{
		string str2 = str;
		std::transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
		return str2;
	}

	std::string toUpperCase(std::string str)
	{
		string str2 = str;
		std::transform(str2.begin(), str2.end(), str2.begin(), ::toupper);
		return str2;
	}

	bool isDigit2(std::string str)
	{
		for (int i = 0; i<str.size(); i++)
		{
			if ((str.at(i)>'9') || (str.at(i) < '0'))
			{
				return false;
			}
		}
		return true;
	}

} // namespace StringUtils {


NS_CC_END