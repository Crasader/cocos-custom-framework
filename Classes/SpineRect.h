#ifndef __SpineRect_H__
#define __SpineRect_H__
#include "SpineFace.h"



class SpineRect :public SpineFace
{
public:
	enum class Align{
		center = 1 << 0,
		top = 1 << 1,
		bottom = 1 << 2,
		left = 1 << 3,
		right = 1 << 4,

		topLeft = top | left,
		topRight = top | right,
		bottomLeft = bottom | left,
		bottomRight = bottom | right,
	};

public:
	SpineRect();
	~SpineRect();
	static SpineRect* create(const std::string &rSFileName, float scale = 1);
	static SpineRect* createUseOwnData(const std::string &rSFileName, float scale = 1);
	static SpineRect* create(const std::string& skeletonDataFile, const std::string& atlasFile, float scale = 1);
	static SpineRect* createWithData(spSkeletonData* skeletonData, bool ownsSkeletonData = false);
	
public:
	
	virtual Rect getBoundingBox(std::vector<spSlot*>* slots = nullptr);//部分部位的 Box
	
	//配合 getBoundingBox 使用
	virtual myExtension::Rectangle getRectangle(Rect rBoundingBox = Rect::ZERO);

	virtual void setPostion(SpineRect::Align alignment,Vec2 pos, std::vector<spSlot*>* slots = nullptr);
	
	virtual void drawDebugBox(std::vector<spSlot*>* slots = nullptr);

	virtual Vec2 calculatePosition(SpineRect::Align align,const Vec2& pos, std::vector<spSlot*>* slots = nullptr);

	virtual void addBoundNodeWithBone(const std::string &boneName, Node* node);//添加节点在某个骨骼位置，并跟随 位置移动
	virtual void removeBoundNode(Node* node, bool removeFromParent = true);//
	virtual void removeBoundNode(const std::string & nodeName, bool removeFromParent = true);//
	virtual void removeBoundNode(const std::string & boneName, const std::string & nodeName, bool removeFromParent = true);//
	virtual Node* findBoneBoundNode(const std::string & boneName, const std::string & nodeName );

	virtual void update(float deltaTime);
protected:
	virtual bool init(const std::string &rSFileName, float scale = 1);
protected:
	std::map<spBone*, Vector<Node*>*> _mapBounds;
public:

};

#endif /* defined(__SpineRect_H__) */
