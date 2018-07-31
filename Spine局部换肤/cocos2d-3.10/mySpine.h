#ifndef __mySpine_H__  
#define __mySpine_H__  
#pragma once
#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>
#include "spine\PolygonBatch.h"
using namespace std;
using namespace cocos2d;
struct spSkeleton;

/*
mySpine
重写drawSkeleton类，在渲染时找出需要换肤slot，用外部的texture来渲染
有三种类型的渲染方式
SP_ATTACHMENT_REGION:是一个矩形图片的渲染，所以直接用类中mUsv来设置uvs
SP_ATTACHMENT_MESH:usv需要计算一下，大概就是计算一下 x = (x-minx)/(maxx-minx), y = (y-miny)/(maxy-miny)
SP_ATTACHMENT_SKINNED_MESH:跟SP_ATTACHMENT_MESH差不多，只是SP_ATTACHMENT_MESH用verticesCount，当前用uvsCount
*/
class mySpine :public spine::SkeletonAnimation
{
public:
	~mySpine() { if (mFloat)delete[]mFloat; }
	mySpine(const std::string& skeletonDataFile, const std::string& atlasFile, float scale = 1);
	mySpine(spSkeletonData* skeletonData, bool ownsSkeletonData = false);

	static mySpine * create(const std::string& skeletonDataFile, const std::string& atlasFile, float scale = 1);
	static mySpine* createWithData(spSkeletonData* skeletonData, bool ownsSkeletonData = false);

	void myInit();
	virtual void drawSkeleton(const Mat4 &transform, uint32_t transformFlags);
	void setSkinFile(string slotName, string filePath);
	void setSkinFile(string slotName, Sprite* rSprite);
	void setSkinFile(string slotName, Texture2D* rTexture2D);
	void setSkinAlphas(string slotName, float alpha);
	void cleanCustomSkin();
	void removeCustomSkin(string slotName);


	int getBonesCount();
	int getSlotsCount();
	spBone** getBones();
	spSlot** getSlots();
private:
	Map<string, Sprite*>useSkin;	//保存需要换肤的slot
	map<string, float> slotAlphas;
	float * mFloat;				//用于SP_ATTACHMENT_MESH和SP_ATTACHMENT_SKINNED_MESH渲染中保存计算出新的usv的值
	float mUsv[8];				//用于SP_ATTACHMENT_REGION的渲染

};

#endif // __mySpine_H__  