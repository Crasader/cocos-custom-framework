#include "BaseLoading.h"
#include "CSAsyncLoader.h"
#include "DataManager.h"

#define ThreadNum 5

BaseLoading::BaseLoading():
_bIsLoading(false),
_bLoadEnd(false)
{
}


BaseLoading::~BaseLoading()
{

	_vJsonDatas.clear();
	_vImgs.clear();
	_vPlists.clear();
	_vCsbs.clear();
	_vSpines.clear();
	_vSounds.clear();

}

void BaseLoading::beginLoad()
{
	_bIsLoading = true;
	loadImg();
	loadJsonData();

}

void BaseLoading::onLoadEnd()
{
	_bIsLoading = false;

	_vJsonDatas.clear();
	_vImgs.clear();
	_vPlists.clear();
	_vCsbs.clear();
	_vSpines.clear();
	_vSounds.clear();
	CCLOG("on load end~~~~~");
}

void BaseLoading::onLoadJsonDataCallBack(const std::string& filename)
{

}

void BaseLoading::loadImg()
{
	for (auto csb : _vCsbs)
	{
		CSAsyncLoader::addCsb(csb);
	}
	int csbImgSize = CSAsyncLoader::getResSize();
	_iTotalImgSize = csbImgSize + _vImgs.size() + _vPlists.size();//ͼƬ��Դ����
	auto pTextureCache = Director::getInstance()->getTextureCache();
	for (auto img : _vImgs){
		pTextureCache->addImageAsync(img, [=](Texture2D *pTexture)
		{
			_iImgCount++;
		});
	}
	for (auto plist : _vPlists){
		std::string texturePath = plist;

		// remove .xxx
		size_t startPos = texturePath.find_last_of(".");
		texturePath = texturePath.erase(startPos);

		// append .png
		texturePath = texturePath.append(".png");
		pTextureCache->addImageAsync(texturePath, [=](Texture2D *pTexture)
		{
			_iImgCount++;
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist, pTexture);
		});
	}
	CSAsyncLoader::load([=](float csbPre){
		_iImgCount++;
		CCLOG("BaseLoading load index -> %d, total -> %d", _iImgCount, _iTotalImgSize);
	});
}

void BaseLoading::loadJsonData()
{
	_iTotalJsonDataSize = _vJsonDatas.size();
	for (int i = 0; i < ThreadNum; i++)
	{
		std::thread loadDataThread(&BaseLoading::loadJsonThread, this, i);//����һ����֧�̣߳��ص���myThread������  
		loadDataThread.detach();
	}
}

void BaseLoading::loadJsonThread(int threadID)
{
	int needLoadSize = std::ceil((float)_iTotalJsonDataSize / ThreadNum);
	int beginId = needLoadSize*threadID;
	int endId = beginId + needLoadSize;

	for (int j = beginId; j < endId; j++)
	{
		if (j >= _vJsonDatas.size())break;

		std::string filename = _vJsonDatas.at(j);
		
		DataManager::getInstance()->readDataFromFileOtherThread(filename.c_str(), &_mutex);

		_mutex.lock();
		onLoadJsonDataCallBack(filename);//���ݼ�����ɵĴ���
		_iJsonDataCount++;
		_mutex.unlock();
	}

}
float BaseLoading::getVisualPercent(float realPercent)
{
	float delta = Director::getInstance()->getDeltaTime();
	int iPercent = realPercent;
	_iVisualPercent = MIN(_iVisualPercent + 20 * delta, iPercent);
	if (_iVisualPercent >= 100){
		if (getJsonDataPercent() >= 100 && getImgPercent() >= 100){
			if (_bLoadEnd == false){
				_bLoadEnd = true;

				onLoadEnd();
			}
		}
	}
	return _iVisualPercent;
}


float BaseLoading::getPercent()
{
	float fPercent = MIN(getJsonDataPercent(), getImgPercent());
	return fPercent;
}

float BaseLoading::getJsonDataPercent()
{
	if (_iTotalJsonDataSize == 0)return 100;
	if (_iJsonDataCount == 0)return 0;
	float fJsonDataPercent = (float)_iJsonDataCount / (float)_iTotalJsonDataSize;
	return fJsonDataPercent * 100;
}

float BaseLoading::getImgPercent()
{
	if (_iTotalImgSize == 0)return 100;
	if (_iImgCount == 0)return 0;
	float fImgPercene = (float)_iImgCount / (float)_iTotalImgSize;
	return fImgPercene * 100;
}

void BaseLoading::checkIsLoading()
{
	CCASSERT(_bIsLoading == false,"is loading , can't add assets");
}

void BaseLoading::addJsonData(const std::string& filename)
{
	checkIsLoading();
	_vJsonDatas.push_back(filename);
}

void BaseLoading::addImg(const std::string& filename)
{
	checkIsLoading();
	_vImgs.push_back(filename);
}

void BaseLoading::addPlist(const std::string& filename)
{
	checkIsLoading();
	_vPlists.push_back(filename);
}

void BaseLoading::addCsb(const std::string& filename)
{
	checkIsLoading();
	_vCsbs.push_back(filename);
}

void BaseLoading::addSpine(const std::string& filename)
{
	checkIsLoading();
	_vSpines.push_back(filename);
}

void BaseLoading::addSound(const std::string& filename)
{
	checkIsLoading();
	_vSounds.push_back(filename);
}
