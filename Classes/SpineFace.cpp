#include "SpineFace.h"
#include <spine/extension.h>


//spine data ---- 缓存
std::unordered_map< std::string, spSkeletonData*> SpineFace::spSkeletonDataMap;

spSkeletonData* SpineFace::create_spSkeletonData_WithFile(const std::string& skeletonDataFile, float scale /*= 1*/)
{
	CCLOG("create_spSkeletonData_WithFile -> %s", skeletonDataFile.c_str());
	auto filename = StringUtil::pathWithoutExtension(skeletonDataFile);
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
	return skeletonData;
}//spine data ---- 缓存


SpineFace::SpineFace()
:mySpine()
{

}

SpineFace::~SpineFace()
{
	anims.clear();
	randomAnims.clear();
	_mRepeatTimes.clear();
}

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

SpineFace* SpineFace::create(const std::string& skeletonDataFile, const std::string& atlasFile, float scale /*= 1*/)
{
	SpineFace *pRet = new(std::nothrow) SpineFace();
	pRet->initWithJsonFile(skeletonDataFile, atlasFile, scale);
	pRet->initData(atlasFile);
	pRet->autorelease();

	return pRet;
}

SpineFace* SpineFace::createUseOwnData(const std::string &rSFileName, float scale /*= 1*/)
{
	SpineFace *pRet = new(std::nothrow) SpineFace();
	auto filename = StringUtil::pathWithoutExtension(rSFileName);
	bool isExist = (spSkeletonDataMap.find(filename) != spSkeletonDataMap.end());//判断map里面是否存在 key  skeletonDataFile
	std::string atlasFile = filename + ".atlas";
	std::string jsonFile = filename + ".json";

	pRet->initWithJsonFile(rSFileName, atlasFile, scale);
	pRet->initData(rSFileName);
	pRet->autorelease();

	return pRet;
}

SpineFace* SpineFace::createWithData(spSkeletonData* skeletonData, bool ownsSkeletonData /*= false*/)
{
	SpineFace* node = new(std::nothrow) SpineFace();
	node->initWithData(skeletonData, ownsSkeletonData);
	node->initData(skeletonData->defaultSkin->name);
	node->autorelease();
	return node;
}



bool SpineFace::init(const std::string &rSFileName, float scale /*= 1*/)
{
	bool bRet = false;
	do
	{
	
		spSkeletonData* skeletonData = create_spSkeletonData_WithFile(rSFileName, scale);
		initWithData(skeletonData, false);

		bRet = initData(rSFileName);
		
	} while (0);

	return bRet;
}


bool SpineFace::initData(const std::string &rSFileName)
{
	bool bRet = false;
	do
	{
		_bIsPlaying = false;

		anims.clear();
		_mRepeatTimes.clear();
		setCascadeColorEnabled(true);
		setCascadeOpacityEnabled(true);
		auto name = StringUtil::nameWithoutExtension(rSFileName);
		setName(name);
		_sSpineStr = rSFileName;

		//allMix();
		bRet = true;
	} while (0);

	return bRet;
}

void SpineFace::setMixFro(const std::string& fromAnimation, const std::string& toAnimation, float duration)
{
	setMix(fromAnimation, toAnimation, duration);
	setMix(toAnimation, fromAnimation, duration);
}


void SpineFace::stopAnim()
{
	_sPlayKey = "";
	spAnimationState_setEmptyAnimation(_state,0,0);
	update(0);
}

bool SpineFace::play(const std::string& name, float timescale /*= 1*/)
{
	setTimeScale(timescale);
	setAnimation(0, name, true);
	_sPlayKey = name;
	_bIsPlaying = true;
	return true;
}

float SpineFace::playOnce(const std::string& name, const SpineFace::OnPlayOnceListener& listener, float timescale /*= 1*/)
{
	
	return playRepeat(name, 1, listener, timescale);
}

float SpineFace::playRepeat(const std::string& name, int times, const SpineFace::OnPlayOnceListener& listener, float timescale /*= 1*/)
{
	_bIsPlaying = true;
	setTimeScale(timescale);
	spTrackEntry* trackEntry = setAnimation(0, name, times == 1 ? false : true);

	if (trackEntry == nullptr){
		if (listener){
			listener(this,nullptr);
		}
		return 0;
	}

	if (_mRepeatTimes.find(name) != _mRepeatTimes.end()){
		_mRepeatTimes.at(name) = 0;
	}
	else{
		_mRepeatTimes.emplace(name, 0);
	}
	setTrackCompleteListener(trackEntry, [=](spTrackEntry* entry){
		 
 		_mRepeatTimes.at(name)++;
 		bool isBigger = _mRepeatTimes.at(name) >= times;
 		if (isBigger){
			_bIsPlaying = false;
 			if (listener){
 				listener(this,entry);
 			}
 		}
	});

	_sPlayKey = name;

	return trackEntry->animation->duration * times;
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
			setMix(namei, namei, 0.1);
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
		}
	}
	return anims;
 }

 void SpineFace::logAnims()
 {
	 std::vector<std::string> anims = getAnims();

	 for (int i = 0; i < anims.size();i++)
	 {
		 auto item = anims.at(i);
		 CCLOG("count %d the anim -> %s",i, item.c_str());
	 }
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

	std::vector<std::string> temp = randomAnims.empty() ? getAnims() : randomAnims;

	auto size = temp.size();

	auto name = _sPlayKey;

	while (name == _sPlayKey && size >= 1)//不让随机到跟当前播放的动画一样的动画
	{
		int index = random(0,(int) size - 1);
		name = temp.at(index);
	}
	return name;

}

void SpineFace::playRandomAnim(int times /*= -1*/, SpineFace::OnPlayOnceListener listener /*= nullptr*/)
{
	auto anim = getRandomAnim();
	if (times <= 0){
		play(anim);
	}
	else{
		playRepeat(anim, times, listener);
	}
}


void SpineFace::setSwallowTouches(bool swallow)
{
	_needSwallow = swallow;
	if (_touchListener){
		_touchListener->setSwallowTouches(swallow);
	}
}

void SpineFace::setTouchEnabled(bool enabled)
{
	if (enabled == _touchEnabled)
	{
		return;
	}
	_touchEnabled = enabled;
	if (_touchEnabled)
	{
		_touchListener = EventListenerTouchOneByOne::create();
		CC_SAFE_RETAIN(_touchListener);
		_touchListener->setSwallowTouches(_needSwallow);
		_touchListener->onTouchBegan = CC_CALLBACK_2(SpineFace::onTouchBegan, this);
		_touchListener->onTouchMoved = CC_CALLBACK_2(SpineFace::onTouchMoved, this);
		_touchListener->onTouchEnded = CC_CALLBACK_2(SpineFace::onTouchEnded, this);
		_touchListener->onTouchCancelled = CC_CALLBACK_2(SpineFace::onTouchCancelled, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
	}
	else
	{
		_eventDispatcher->removeEventListener(_touchListener);
		CC_SAFE_RELEASE_NULL(_touchListener);
	}
}

bool SpineFace::onTouchBegan(Touch *touch, Event *unusedEvent)
{
	Point touchPoint = touch->getLocation();
	if (isInBoundingBox(touchPoint)){
		return true;
	}
	return false;
}

void SpineFace::onTouchMoved(Touch *touch, Event *unusedEvent)
{

}

void SpineFace::onTouchEnded(Touch *touch, Event *unusedEvent)
{

	Point touchPoint = touch->getLocation();
	if (isInBoundingBox(touchPoint)){
		_clickEventListener(this);
	}
}

void SpineFace::onTouchCancelled(Touch *touch, Event *unusedEvent)
{

}

void SpineFace::addClickListener(const mySpineClickCallback &callback)
{
	this->_clickEventListener = callback;
}

bool SpineFace::isInBoundingBox(Point point)
{

	Rect nrect = this->getBoundingBox();
	nrect.origin = this->getParent()->convertToWorldSpace(nrect.origin);
	auto worldBox = nrect;
	if (worldBox.containsPoint(point)){
		return true;
	}
	return false;
}

myExtension::Rectangle SpineFace::getRectangle()
{
	Vec2 position = this->getPosition();
	Rect box = this->getBoundingBox();
	float _minx = box.origin.x - position.x;
	float _miny = box.origin.y - position.y;
	float _maxx = box.size.width + _minx;
	float _maxy = box.size.height + _miny;

	
	myExtension::Rectangle rectangle(_minx, _maxy, _maxx, _miny);
	return rectangle;
}
