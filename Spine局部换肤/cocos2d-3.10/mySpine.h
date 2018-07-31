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
��дdrawSkeleton�࣬����Ⱦʱ�ҳ���Ҫ����slot�����ⲿ��texture����Ⱦ
���������͵���Ⱦ��ʽ
SP_ATTACHMENT_REGION:��һ������ͼƬ����Ⱦ������ֱ��������mUsv������uvs
SP_ATTACHMENT_MESH:usv��Ҫ����һ�£���ž��Ǽ���һ�� x = (x-minx)/(maxx-minx), y = (y-miny)/(maxy-miny)
SP_ATTACHMENT_SKINNED_MESH:��SP_ATTACHMENT_MESH��ֻ࣬��SP_ATTACHMENT_MESH��verticesCount����ǰ��uvsCount
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
	Map<string, Sprite*>useSkin;	//������Ҫ������slot
	map<string, float> slotAlphas;
	float * mFloat;				//����SP_ATTACHMENT_MESH��SP_ATTACHMENT_SKINNED_MESH��Ⱦ�б��������µ�usv��ֵ
	float mUsv[8];				//����SP_ATTACHMENT_REGION����Ⱦ

};

#endif // __mySpine_H__  