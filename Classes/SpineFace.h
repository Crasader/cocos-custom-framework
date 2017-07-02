#ifndef __SPINEFACE_H__
#define __SPINEFACE_H__
#include "mySpine.h"
#include "myGeometry.h"

typedef std::function<void(spTrackEntry* entry)> OnPlayOnceListener;
class SpineFace :public mySpine
{
	typedef std::function<void(Ref*)> mySpineClickCallback;
public:
	SpineFace();
	~SpineFace();
	static SpineFace* create(const std::string &rSFileName, float scale = 1);
	static SpineFace* createUseOwnData(const std::string &rSFileName, float scale = 1);
	
	/**
	 * the spine play key
	 */
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _sPlayKey, SPlayKey);
	CC_SYNTHESIZE_PASS_BY_REF(bool, _bIsPlaying, IsPlaying);//�Ƿ����ڲ��Ŷ���
public:
	virtual void stopAnim();

	virtual bool play(const std::string& name,float timescale = 1);

	virtual float playOnce(const std::string& name, const OnPlayOnceListener& listener, float timescale = 1);

	virtual float playRepeat(const std::string& name, int times, const OnPlayOnceListener& listener, float timescale = 1);

	virtual bool isPlay(const std::string& name);

	virtual void setFlipX(bool flipX);
	virtual void setFlipY(bool flipY);

	virtual bool getFlipX();
	virtual bool getFlipY();
	
	virtual void setRandomAnims(std::vector<std::string> anims);
	virtual std::vector<std::string> getAnims();
	virtual std::string getRandomAnim();
	virtual void playRandomAnim(int times = -1, OnPlayOnceListener listener = nullptr);
	virtual void allMix();

	virtual c2d::Rectangle getRectangle();

	virtual bool isTouchEnabled(){ return _touchEnabled; };
	virtual void setTouchEnabled(bool enabled);

	virtual bool onTouchBegan(Touch *touch, Event *unusedEvent);

	virtual void onTouchMoved(Touch *touch, Event *unusedEvent);

	virtual void onTouchEnded(Touch *touch, Event *unusedEvent);

	virtual void onTouchCancelled(Touch *touch, Event *unusedEvent);

	virtual void addClickListener(const mySpineClickCallback &callback);
protected:

	virtual bool init(const std::string &rSFileName, float scale = 1);
	virtual bool initData(const std::string &rSFileName);
	virtual void setMixFro(const std::string& fromAnimation, const std::string& toAnimation, float duration);
	
	virtual bool isInBoundingBox(Point point);
	
protected:
	std::string _sSpineStr;
	std::vector<std::string> anims;
	std::vector<std::string> randomAnims;
	std::unordered_map<std::string, int> _mRepeatTimes;
	EventListenerTouchOneByOne* _touchListener = nullptr;
	bool _touchEnabled = false;
	mySpineClickCallback _clickEventListener;
public:
	static std::unordered_map< std::string, spSkeletonData*>  spSkeletonDataMap;
	static spSkeletonData* create_spSkeletonData_WithFile(const std::string& skeletonDataFile, float scale = 1);
};

#endif /* defined(__SPINEFACE_H__) */
