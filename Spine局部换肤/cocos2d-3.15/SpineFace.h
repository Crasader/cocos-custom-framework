#ifndef __SPINEFACE_H__
#define __SPINEFACE_H__
#include "mySpine.h"
#include "StringUtil.h"

typedef std::function<void(spTrackEntry* entry)> OnPlayOnceListener;
class SpineFace :public mySpine
{
public:
	SpineFace();

	static SpineFace* create(const std::string &rSFileName, float scale = 1);
	
	
	/**
	 * the spine play key
	 */
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _sPlayKey, SPlayKey);

public:
	
	virtual bool play(const std::string& name,float timescale = 1);

	virtual float playOnce(const std::string& name, const OnPlayOnceListener& listener, float timescale = 1);

	virtual bool isPlay(const std::string& name);

	virtual void setFlipX(bool flipX);
	virtual void setFlipY(bool flipY);

	virtual bool getFlipX();
	virtual bool getFlipY();
	
	virtual void setRandomAnims(std::vector<std::string> anims);
	virtual std::vector<std::string> getAnims();
	virtual std::string getRandomAnim();
	virtual void playRandomAnim();
	virtual void allMix();
protected:

	virtual bool init(const std::string &rSFileName, float scale = 1);
	
	virtual void setMixFro(const std::string& fromAnimation, const std::string& toAnimation, float duration);
	


private:
	int getRand(int nstart, int nend);
private:
	std::string _sSpineStr;
	std::vector<std::string> anims;
	std::vector<std::string> randomAnims;




public:
	static std::unordered_map< std::string, spSkeletonData*>  spSkeletonDataMap;
	static spSkeletonData* create_spSkeletonData_WithFile(const std::string& skeletonDataFile, float scale = 1);
};

#endif /* defined(__SPINEFACE_H__) */
