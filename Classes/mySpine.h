#ifndef __mySpine_H__  
#define __mySpine_H__  
#pragma once
#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>

using namespace std;
using namespace cocos2d;
struct spSkeleton;
using namespace spine;
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
	mySpine();
	~mySpine();

	CREATE_FUNC(mySpine);

	static mySpine * create(const std::string& skeletonDataFile, const std::string& atlasFile, float scale = 1);
	static mySpine* createWithData(spSkeletonData* skeletonData, bool ownsSkeletonData = false);
	
	
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformFlags);
	//virtual void drawSkeleton(const Mat4 &transform, uint32_t transformFlags);
	virtual void setSkinFile(const std::string& slotName, const std::string& filePath);
	virtual void setSkinFileWithData(const std::string& slotName, const std::string& filePath);
	virtual void setSkinFile(const std::string& slotName, Sprite* rSprite);
	virtual void setSkinFile(const std::string& slotName, Texture2D* rTexture2D);
	virtual void setSkinAlphas(const std::string& slotName, float alpha, float duration = 0);//duration 是过渡时间

	virtual void setCustomAttachment(const std::string& slotName, const std::string attachmentName,const std::string& filepath);
	virtual void removeCustomAttachment(const std::string& slotName, const std::string attachmentName);

	virtual void cleanCustomSkin();
	virtual void removeCustomSkin(const std::string& slotName);
	virtual void removeSkinAlphas(const std::string& slotName);

	virtual map<string, float> getSlotAlphas(){ return slotAlphas; };

	virtual int getBonesCount();
	virtual int getSlotsCount();
	spBone** getBones();
	spSlot** getSlots();

	virtual void logSlots();
	virtual void logAttachments();
protected:
	virtual void copyTriangles(cocos2d::TrianglesCommand::Triangles* target, cocos2d::TrianglesCommand::Triangles* source);
private:
	Map<string, Sprite*>useSkin;	//保存需要换肤的slot
	map<string, float> slotAlphas;
	map<string, float> visualSlotAlphas;
	map<string, float> durations;
	cocos2d::TrianglesCommand::Triangles* _pTriangles = nullptr;

	std::map<std::string, Map<std::string, Sprite*>> useAttachment; // 一个附件一张图片
};

#endif // __mySpine_H__  