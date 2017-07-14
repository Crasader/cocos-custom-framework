#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PublicDefine.h"
#include "VisibleRect.h"
#include "SpineRect.h"
#include "Align.h"
#include "myStringUtils.h"
#include "KeysAgent.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
   
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
//     auto rootNode = CSLoader::createNode("MainScene.csb");
// 
//     addChild(rootNode);
// 
// 	EditBox* editor = EditBox::create(Size(234,60),"E206.png");
// 	editor->setFont("miaowu.ttf", 25);
// 	
// 	editor->setPosition(VisibleRect::center());
// 	addChild(editor);



	auto spinf = SpineRect::create("fox.json");
	addChild(spinf);
	spinf->setPostion(c2d::Align::center, VisibleRect::center());
	//spinf->setPosition( VisibleRect::center());
	spinf->play("zou");

	auto bone = spinf->findBone("bone11");
	DELOG(__FILE__, __LINE__, "the pos -> %02f,%02f", bone->worldX, bone->worldY);

	auto qiqiu = Sprite::create("foxqq.png");
	qiqiu->setAnchorPoint(Vec2(0.75,0));
//  	spinf->addChild(qiqiu);
//  	qiqiu->setPosition(Vec2(bone->worldX, bone->worldY));
	spinf->addBoundNodeWithBone("bone11", qiqiu);


	KeysAgent::registered(this, [=](){
		spinf->removeBoundNode(qiqiu,false);
	}, EventKeyboard::KeyCode::KEY_W);
    return true;
}
