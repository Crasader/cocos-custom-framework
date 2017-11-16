
#ifndef _PublicCoco_h_
#define _PublicCoco_h_
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"


#include <vector>
#include<map>
#include <iostream>
#include<string>
#include<cstring>



#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

using namespace std;
using namespace spine;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;
using namespace cocostudio::timeline;


#endif
