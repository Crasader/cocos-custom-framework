#ifndef _Audio__Manager__
#define _Audio__Manager__

#include "cocos2d.h"
#include "AudioEngine.h"


USING_NS_CC;
using namespace std;
using namespace cocos2d::experimental;

class AudioManager
{
public:
	static AudioManager* getInstance();
	static void clickBtn();
	bool init();
	void PlayBackgroundmusic(const std::string& nstr, float volume = 0.5f);
	int PlayVoiceEffect(const std::string& nstr);
	int PlayVoiceUniqueness(const std::string& eleid);
	void voiceEndCallback(int id, const std::string& filePath);
	void preloadVoice(const std::string& nstr);
	bool isPlaying(int id);
	bool isPlaying(const std::string& nstr);
	void stopVoice(const std::string& nstr);

	void stopCurVoice();
	void stopAllVoice();
	void resumeAllVoice();
	void restoreBMG();
	void stopAll();

	void pauseAll();
	void resumeAll();

	void setBackgroundVolume(float volume);

	static bool isMusicOn();
	static bool isSoundOn();
	static void setMusicOn(bool on);
	static void setSoundOn(bool on);
	static int getMusicValue();
	static int getSoundValue();
	static void setMusicValue(int value);
	static void setSoundValue(int value);

protected:
	int curBgmId = -1;
	int curVoiceId = -1;
	float cacheBMGvolume = 0.5f;
	bool _bIsStopBGM = false;
	std::string curBgmStr;
	std::string formatStr(const string &str);

	std::vector<int> voiceIdVec;
	std::map<std::string, int> _mSoundMap;
};

#endif /* defined(_Audio__Manager__) */
