#include "AudioManager.h"


static AudioManager *_globaldata;
static int btnId = -1;
AudioManager * AudioManager::getInstance()
{
	if (!_globaldata)
	{
		_globaldata = new AudioManager();
		_globaldata->init();
	}
	return _globaldata;
}

void AudioManager::clickBtn()
{
	//btnId = getInstance()->PlayVoiceEffect(mp3_button);
}

bool AudioManager::init()
{
	_bIsStopBGM = !isMusicOn();
	curBgmStr = "";
	return true;
}
void AudioManager::PlayBackgroundmusic(const std::string& nstr, float volume /*= 0.5f*/)
{
	bool nMusicOn = isMusicOn();
	float musicValue = getMusicValue();
	if (!_bIsStopBGM){
		if (curBgmStr == nstr)return;
	}
	if (!nMusicOn)
	{
		musicValue = 0;
	}
	
	_bIsStopBGM = false;
	if ((nstr != "~")&(nstr != ""))
	{

		if (curBgmId != -1)
		{
			AudioEngine::stop(curBgmId);
		}

		curBgmId = AudioEngine::play2d(formatStr(nstr).c_str(), true, musicValue / 100.0 * volume);
	}
	
	 
	cacheBMGvolume = volume;
	curBgmStr = nstr;
}
int AudioManager::PlayVoiceEffect(const std::string& nstr)
{
	bool nMusicOn = isSoundOn();
	float nEffectValue = getSoundValue();
	if (!nMusicOn)
	{
		nEffectValue = 0;
	}

	if ((nstr != "~")&(nstr != ""))
	{
		curVoiceId = AudioEngine::play2d(formatStr(nstr).c_str(), false, nEffectValue / 100.0);
		if (curVoiceId != -1)
		{
			voiceIdVec.push_back(curVoiceId);
			_mSoundMap[nstr] = curVoiceId;
		}
	}
	
	return curVoiceId;
}
//通过 元素名称 播放音效
int AudioManager::PlayVoiceUniqueness(const std::string& nstr)
{
	auto data = nstr;
 	bool nMusicOn = isSoundOn();
	float nEffectValue = getSoundValue();
	if (!nMusicOn)
	{
		nEffectValue = 0;
	}
	if ((nstr != "~")&(nstr != ""))
	{
		CCLOG("current the sound id -->%d", curVoiceId);
		if (curVoiceId != -1){
			CCLOG("stop the sound id -->%d", curVoiceId);
			AudioEngine::stop(curVoiceId);
		}
		CCLOG("play sound the voice name---->%s", formatStr(data).c_str());
		
		curVoiceId = AudioEngine::play2d(formatStr(data).c_str(), false, nEffectValue / 100.0);

		CCLOG("play the sound id -->%d", curVoiceId);
		if (curVoiceId != -1)
		{
			voiceIdVec.push_back(curVoiceId);
			_mSoundMap[nstr] = curVoiceId;
		}
	}
	

	return curVoiceId;
}

bool AudioManager::isPlaying(int id)
{
	if (id == -1)return false;

	auto state = AudioEngine::getState(id);
	return state == AudioEngine::AudioState::PLAYING;
}

bool AudioManager::isPlaying(const std::string& nstr)
{
	return isPlaying(_mSoundMap[nstr]);
}


void AudioManager::stopVoice(const std::string& nstr)
{
	if (isPlaying(_mSoundMap[nstr])){
		AudioEngine::stop(_mSoundMap[nstr]);
	}
}

void AudioManager::voiceEndCallback(int id, const std::string& filePath)
{ 
}
//预加载音效
void AudioManager::preloadVoice(const std::string& nstr)
{
	AudioEngine::preload(formatStr(nstr).c_str());
}


void AudioManager::stopAllVoice()
{
	for (auto item : voiceIdVec)
	{
		if (item == btnId)continue;
		AudioEngine::stop(item);
	}
	
	//voiceIdVec.clear();
}

void AudioManager::resumeAllVoice()
{
	for (auto item : voiceIdVec)
	{
		//if (item == btnId)continue;
		AudioEngine::setVolume(item, 1);
	}
}

void AudioManager::stopCurVoice()
{
	if (curVoiceId != -1 && curVoiceId != btnId){
		CCLOG("stop the sound id -->%d", curVoiceId);
		AudioEngine::stop(curVoiceId);
		curVoiceId = -1;
	}
}
void AudioManager::stopAll()
{
	_bIsStopBGM = true;
	AudioEngine::stopAll();
	//AudioEngine::setVolume(curBgmId, 0);
}

void AudioManager::pauseAll()
{
	AudioEngine::pauseAll();
}

void AudioManager::resumeAll()
{
	AudioEngine::resumeAll();
}

void AudioManager::restoreBMG()
{
	_bIsStopBGM = true;
	PlayBackgroundmusic(curBgmStr, cacheBMGvolume);
}

void AudioManager::setBackgroundVolume(float volume)
{
	AudioEngine::setVolume(curBgmId, volume);
}


std::string AudioManager::formatStr(const std::string &str)
{
	std::string temp = str;
	if (str.find(".ogg") < str.length() || str.find(".mp3") < str.length()){
		//包含ogg字段
		temp = str;
	}
	else{
		temp = StringUtils::format("%s.mp3", str.c_str());
	}
	CCLOG("format ogg str ---->%s", temp.c_str());
	return temp;
}
bool AudioManager::isMusicOn()
{
	return UserDefault::getInstance()->getBoolForKey("MusicOn", true);
}

bool AudioManager::isSoundOn()
{
	return UserDefault::getInstance()->getBoolForKey("SoundOn", true);
}

int AudioManager::getMusicValue()
{
	return  UserDefault::getInstance()->getIntegerForKey("MusicValue", 100);
}

int AudioManager::getSoundValue()
{
	return UserDefault::getInstance()->getIntegerForKey("SoundValue", 100);
}

void AudioManager::setMusicValue(int value)
{
	UserDefault::getInstance()->setIntegerForKey("MusicValue", value);
}

void AudioManager::setSoundValue(int value)
{
	UserDefault::getInstance()->setIntegerForKey("SoundValue", value);
}

void AudioManager::setMusicOn(bool on)
{
	UserDefault::getInstance()->setBoolForKey("MusicOn", on);
}

void AudioManager::setSoundOn(bool on)
{
	UserDefault::getInstance()->setBoolForKey("SoundOn", on);
}
