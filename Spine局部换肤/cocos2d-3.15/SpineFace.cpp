#include "SpineFace.h"
#include <spine/extension.h>



SpineFace* SpineFace::create(const std::string &rSFileName, float scale /*= 1*/)
{
	SpineFace *pRet = new(std::nothrow) SpineFace();
	if (pRet && pRet->init(rSFileName, scale))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}



SpineFace::SpineFace()
:mySpine()
{

}

bool SpineFace::init(const std::string &rSFileName, float scale /*= 1*/)
{
	bool bRet = false;
	do
	{
		spSkeletonData* skeletonData = create_spSkeletonData_WithFile(rSFileName, scale);
		initWithData(skeletonData, false);

		anims.clear();
		setCascadeColorEnabled(true);
		setCascadeOpacityEnabled(true);
		_sSpineStr = rSFileName;
		bRet = true;
		allMix();
	} while (0);

	return bRet;
}


void SpineFace::setMixFro(const std::string& fromAnimation, const std::string& toAnimation, float duration)
{
	CCLOG("the mix ");
	setMix(fromAnimation, toAnimation, duration);
	setMix(toAnimation, fromAnimation, duration);
}


bool SpineFace::play(const std::string& name, float timescale /*= 1*/)
{

	setTimeScale(timescale);
	setAnimation(0, name, true);
	_sPlayKey = name;
	return true;
}

float SpineFace::playOnce(const std::string& name, const OnPlayOnceListener& listener, float timescale /*= 1*/)
{

	setTimeScale(timescale);
	spTrackEntry* trackEntry = setAnimation(0, name, false);
	
	setTrackCompleteListener(trackEntry, [=](spTrackEntry* entry){
		if (listener){
			listener(entry);
		}
	});

	_sPlayKey = name;

	return trackEntry->animation->duration;
}

bool SpineFace::isPlay(const std::string& rKey)
{
	return _sPlayKey.compare(rKey) == 0;
}

void SpineFace::allMix()
{
	auto self = _skeleton->data;
	spAnimation** animations = self->animations;

	for (int i = 0; i < self->animationsCount; ++i){
		for (int j = i+1; j < self->animationsCount; ++j){
			auto namei = animations[i]->name;
			auto namej = animations[j]->name;
			setMixFro(namei, namej, 0.1);
		}
	}
}

void SpineFace::setFlipX(bool flipX)
{
	_skeleton->flipX = flipX;
}

void SpineFace::setFlipY(bool flipY)
{
	_skeleton->flipY = flipY;
}

bool SpineFace::getFlipX()
{
	return _skeleton->flipX;
}

bool SpineFace::getFlipY()
{
	return _skeleton->flipY;
}

 std::vector<std::string> SpineFace::getAnims()
 {
	if (anims.empty()){
		auto data = _skeleton->data;
		for (int i = 0; i < data->animationsCount; i++)
		{
		 anims.push_back(data->animations[i]->name);
		 CCLOG("the anim name is -> %s", data->animations[i]->name);
		}
	}
	return anims;
 }

 void SpineFace::setRandomAnims(std::vector<std::string> anims)
 {
	 for (auto item : anims)
	 {
		 randomAnims.push_back(item);
	 }
 }

std::string SpineFace::getRandomAnim()
{
	std::vector<std::string> temp = randomAnims.empty() ?  getAnims() :  randomAnims;

	int index = getRand(0, temp.size() - 1);

	auto name = temp.at(index);

	CCLOG("the random anim name is -> %s",name.c_str());
	return name;

}

void SpineFace::playRandomAnim()
{	
	play(getRandomAnim());
}



std::unordered_map< std::string, spSkeletonData*> SpineFace::spSkeletonDataMap;

spSkeletonData* SpineFace::create_spSkeletonData_WithFile(const std::string& skeletonDataFile, float scale /*= 1*/)
{
	
	auto filename = StringUtil::nameWithoutExtension(skeletonDataFile);
	bool isExist = (spSkeletonDataMap.find(filename) != spSkeletonDataMap.end());//判断map里面是否存在 key  skeletonDataFile
	std::string atlasFile = filename + ".atlas";
	std::string jsonFile = filename + ".json";

	spSkeletonData * skeletonData = nullptr;

	if (isExist){
		skeletonData = spSkeletonDataMap.at(filename);
		log("skeletonData exist");
	}
	else{
		auto _atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
		CCASSERT(_atlas, "Error reading atlas file.");

		auto _attachmentLoader = SUPER(Cocos2dAttachmentLoader_create(_atlas));

		spSkeletonJson* json = spSkeletonJson_createWithLoader(_attachmentLoader);
		json->scale = scale;
		skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile.c_str());
		spSkeletonDataMap[filename] = skeletonData;
		CCASSERT(skeletonData, json->error ? json->error : "Error reading skeleton data file.");
		spSkeletonJson_dispose(json);
	}
	//-------------------
	return skeletonData;
}

int SpineFace::getRand(int start, int end)
{
	float i = CCRANDOM_0_1()*((end + 1) - start) + start;  //产生一个从start到end间的随机数  
	int value = (int)i;

	if (value > end){
		value = end;
	}
	return value;
}
