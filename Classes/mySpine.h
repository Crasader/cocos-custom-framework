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
��дdrawSkeleton�࣬����Ⱦʱ�ҳ���Ҫ����slot�����ⲿ��texture����Ⱦ
���������͵���Ⱦ��ʽ
SP_ATTACHMENT_REGION:��һ������ͼƬ����Ⱦ������ֱ��������mUsv������uvs
SP_ATTACHMENT_MESH:usv��Ҫ����һ�£���ž��Ǽ���һ�� x = (x-minx)/(maxx-minx), y = (y-miny)/(maxy-miny)
SP_ATTACHMENT_SKINNED_MESH:��SP_ATTACHMENT_MESH��ֻ࣬��SP_ATTACHMENT_MESH��verticesCount����ǰ��uvsCount
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
	virtual void setSkinAlphas(const std::string& slotName, float alpha, float duration = 0);//duration �ǹ���ʱ��
	virtual void cleanCustomSkin();
	virtual void removeCustomSkin(const std::string& slotName);


	virtual map<string, float> getSlotAlphas(){ return slotAlphas; };

	virtual int getBonesCount();
	virtual int getSlotsCount();
	spBone** getBones();
	spSlot** getSlots();

protected:
	virtual void copyTriangles(cocos2d::TrianglesCommand::Triangles* target, cocos2d::TrianglesCommand::Triangles* source);
private:
	Map<string, Sprite*>useSkin;	//������Ҫ������slot
	map<string, float> slotAlphas;
	map<string, float> visualSlotAlphas;
	map<string, float> durations;
	cocos2d::TrianglesCommand::Triangles* _pTriangles = nullptr;

};

#endif // __mySpine_H__  