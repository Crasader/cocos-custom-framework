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
	
	virtual Rect getBoundingBox(std::vector<spSlot*>* slots = nullptr);//部分部位的 Box
	
	//配合 getBoundingBox 使用
	virtual c2d::Rectangle getRectangle(Rect rBoundingBox = Rect::ZERO);

	virtual void setPostion(c2d::Align alignment,Vec2 pos, std::vector<spSlot*>* slots = nullptr);
	
	virtual void drawDebugBox(std::vector<spSlot*>* slots = nullptr);

	virtual Vec2 calculatePosition(c2d::Align align, const Vec2& pos, std::vector<spSlot*>* slots = nullptr);
protected:
	virtual bool init(const std::string &rSFileName, float scale = 1);
protected:
public:

};

#endif /* defined(__SpineRect_H__) */
