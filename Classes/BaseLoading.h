#ifndef __BASELOADING_H__
#define __BASELOADING_H__

#include "cocos2d.h"
USING_NS_CC;

class BaseLoading  
{
public:
	BaseLoading();
	~BaseLoading();

	virtual void addJsonData(const std::string& filename);
	virtual void addImg(const std::string& filename);
	virtual void addPlist(const std::string& filename);
	virtual void addCsb(const std::string& filename);
	virtual void addSpine(const std::string& filename);
	virtual void addSound(const std::string& filename);

	virtual void beginLoad();

	virtual void onLoadEnd();

	virtual void onLoadJsonDataCallBack(const std::string& filename);

	virtual float getVisualPercent(float realPercent);// percent   Percent value from 1 to 100.
	virtual float getPercent();// percent   Percent value from 1 to 100.
	virtual float getJsonDataPercent();// percent   Percent value from 1 to 100.
	virtual float getImgPercent();// percent   Percent value from 1 to 100.
protected:
	void checkIsLoading();
	virtual void loadImg();
	virtual void loadJsonData();
	virtual void loadJsonThread(int threadID);
	
protected:
	bool _bLoadEnd = false;
	bool _bIsLoading = false;

	std::vector<std::string> _vJsonDatas;
	std::vector<std::string> _vImgs;
	std::vector<std::string> _vPlists;
	std::vector<std::string> _vCsbs;
	std::vector<std::string> _vSpines;
	std::vector<std::string> _vSounds;


	int _iTotalImgSize = 0;
	int _iImgCount = 0;

	int _iTotalJsonDataSize = 0;
	int _iJsonDataCount = 0;

	float _iVisualPercent = 0;

	std::mutex _mutex;
};

#endif /* defined(__BASELOADING_H__) */
