#ifndef __SpineRect_H__
#define __SpineRect_H__
#include "SpineFace.h"
#include "Align.h"


class SpineRect :public SpineFace
{
public:

public:
	SpineRect();
	~SpineRect();
	static SpineRect* create(const std::string &rSFileName, float scale = 1);
	static SpineRect* createUseOwnData(const std::string &rSFileName, float scale = 1);
	
public:
	
	virtual Rect getBoundingBox(std::vector<spSlot*>* slots = nullptr);//���ֲ�λ�� Box
	
	//��� getBoundingBox ʹ��
	virtual c2d::Rectangle getRectangle(Rect rBoundingBox = Rect::ZERO);

	virtual void setPostion(c2d::Align alignment,Vec2 pos, std::vector<spSlot*>* slots = nullptr);
	
	virtual void drawDebugBox(std::vector<spSlot*>* slots = nullptr);

	virtual Vec2 calculatePosition(c2d::Align align, const Vec2& pos, std::vector<spSlot*>* slots = nullptr);

	virtual void addBoundNodeWithBone(const std::string &boneName,Node* node);//��ӽڵ���ĳ������λ�ã������� λ���ƶ�
	virtual void removeBoundNode(Node* node,bool removeFromParent = true);//
	virtual void removeBoundNode(const std::string & nodeName, bool removeFromParent = true);//
	virtual void removeBoundNode(const std::string & boneName, const std::string & nodeName, bool removeFromParent = true);//
	
	
	virtual void update(float deltaTime);
protected:
	virtual bool init(const std::string &rSFileName, float scale = 1);
protected:

	std::map<spBone*, Vector<Node*>*> _mapBounds;
public:

};

#endif /* defined(__SpineRect_H__) */
