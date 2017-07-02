#include "SpineRect.h"
#include <spine/extension.h>
#include "PublicDefine.h"
#include "myStringUtils.h"


SpineRect::SpineRect()
:SpineFace()
{
	
}

SpineRect::~SpineRect()
{
	
}

SpineRect* SpineRect::create(const std::string &rSFileName, float scale /*= 1*/)
{
	SpineRect *pRet = new(std::nothrow) SpineRect();
	if (pRet && pRet->init(rSFileName, scale))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

SpineRect* SpineRect::createUseOwnData(const std::string &rSFileName, float scale /*= 1*/)
{
	SpineRect *pRet = new(std::nothrow) SpineRect();
	auto filename = c2d::pathWithoutExtension(rSFileName);
	bool isExist = (spSkeletonDataMap.find(filename) != spSkeletonDataMap.end());//判断map里面是否存在 key  skeletonDataFile
	std::string atlasFile = filename + ".atlas";
	std::string jsonFile = filename + ".json";

	pRet->initWithJsonFile(rSFileName, atlasFile, scale);
	pRet->initData(rSFileName);
	pRet->autorelease();

	return pRet;
}


bool SpineRect::init(const std::string &rSFileName, float scale /*= 1*/)
{
	bool bRet = false;
	do
	{
		bRet = SpineFace::init(rSFileName, scale);
		
	} while (0);

	return bRet;
}

Rect SpineRect::getBoundingBox(std::vector<spSlot*>* slots /*= nullptr*/)
{
	std::vector<spSlot*> _ownSlots;
	if (slots == nullptr){
		for (int i = 0; i < _skeleton->slotsCount; ++i) {
			spSlot* slot = _skeleton->slots[i];
			_ownSlots.push_back(slot);
		}
	}
	else{
		_ownSlots = *slots;
	}
	float minX = FLT_MAX, minY = FLT_MAX, maxX = -FLT_MAX, maxY = -FLT_MAX;
	float scaleX = getScaleX(), scaleY = getScaleY();
	for (int i = 0; i < _ownSlots.size(); ++i) {
		spSlot* slot = _ownSlots.at(i);
		if (!slot->attachment) continue;
		int verticesCount;
		if (slot->attachment->type == SP_ATTACHMENT_REGION) {
			spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
			spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
			verticesCount = 8;
		}
		else if (slot->attachment->type == SP_ATTACHMENT_MESH) {
			spMeshAttachment* mesh = (spMeshAttachment*)slot->attachment;
			spMeshAttachment_computeWorldVertices(mesh, slot, _worldVertices);
			verticesCount = mesh->super.worldVerticesLength;
		}
		else
			continue;
		for (int ii = 0; ii < verticesCount; ii += 2) {
			float x = _worldVertices[ii] * scaleX, y = _worldVertices[ii + 1] * scaleY;
			minX = std::min(minX, x);
			minY = std::min(minY, y);
			maxX = std::max(maxX, x);
			maxY = std::max(maxY, y);
		}
	}
	Vec2 position = getPosition();
	if (minX == FLT_MAX) minX = minY = maxX = maxY = 0;
	return Rect(position.x + minX, position.y + minY, maxX - minX, maxY - minY);
}

c2d::Rectangle SpineRect::getRectangle(Rect rBoundingBox /*= Rect::ZERO*/)
{
	Vec2 position = this->getPosition();
	Rect box = rBoundingBox.equals(Rect::ZERO) ? this->getBoundingBox() : rBoundingBox;
	float _minx = box.origin.x - position.x;
	float _miny = box.origin.y - position.y;
	float _maxx = box.size.width + _minx;
	float _maxy = box.size.height + _miny;

	c2d::Rectangle rectangle(_minx, _maxy, _maxx, _miny);
	return rectangle;
}

void SpineRect::drawDebugBox(std::vector<spSlot*>* slots /*= nullptr*/)
{
	this->update(0);//需要先更新骨骼
	Rect rect = this->getBoundingBox(slots);
	c2d::Rectangle rectangle = this->getRectangle(rect);
	auto drawnode = DrawNode::create();
	Vec2 point[4];
	point[0] = Vec2(rectangle.left, rectangle.bottom);
	point[1] = Vec2(rectangle.left, rectangle.top);
	point[2] = Vec2(rectangle.right, rectangle.top);
	point[3] = Vec2(rectangle.right, rectangle.bottom);
	drawnode->drawPolygon(point, 4, Color4F(1, 0, 0, 0), 1, Color4F(0, 1, 0, 1));

	this->addChild(drawnode);
}

void SpineRect::setPostion(c2d::Align alignment, Vec2 pos, std::vector<spSlot*>* slots /*= nullptr*/)
{
	auto finalPos = calculatePosition(alignment, pos, slots);
	this->setPosition(finalPos);
}

cocos2d::Vec2 SpineRect::calculatePosition(c2d::Align alignment, const Vec2& pos, std::vector<spSlot*>* slots /*= nullptr*/)
{
	Rect partsBoundingbox = this->getBoundingBox(slots);
	c2d::Rectangle partsRectangle = this->getRectangle(partsBoundingbox);
	Rect inNodePos = partsRectangle.toRect();
	
	float x = inNodePos.origin.x;
	float y = inNodePos.origin.y;
	float width = inNodePos.size.width;
	float height = inNodePos.size.height;

	
	if (((int)alignment & (int)c2d::Align::right) != 0)
		x += width;
	else if (((int)alignment & (int)c2d::Align::left) == 0) //
		x += width / 2;

	if (((int)alignment & (int)c2d::Align::top) != 0)
		y += height;
	else if (((int)alignment & (int)c2d::Align::bottom) == 0) //
		y += height / 2;


	Vec2 _temPoint(x,y);
	auto finalPos = pos - _temPoint;

	return finalPos;
}
