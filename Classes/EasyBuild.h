#ifndef __EasyBuild_H__
#define __EasyBuild_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>
USING_NS_CC;

using namespace std;
using namespace spine;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;
using namespace cocostudio::timeline;

class EasyBuild  
{
public:
	static Sprite* toSprite(Ref* rNode);
	static ImageView* toImageView(Ref* rNode, bool touchEnbale = false);
	static Button* toButton(Ref* rNode);
	static CheckBox* toCheckBox(Ref* rNode);
	static LoadingBar* toLoadingBar(Ref* rNode, int percent = 100);
	static Layout* toLayout(Ref* rNode);
	static Layer* toLayer(Ref* rNode);
	static Node* toNode(Ref* rNode);
	static Label* toLabel(Ref* rNode);
	static TextAtlas* toTextAtlas(Ref* rNode);
	static ParticleSystemQuad* toParticle(Ref* rNode,bool play = true);
private:
   
};

#endif /* defined(__EasyBuild_H__) */
