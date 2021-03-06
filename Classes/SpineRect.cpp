﻿#include "SpineRect.h"
#include <spine/extension.h>



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

SpineRect* SpineRect::create(const std::string& skeletonDataFile, const std::string& atlasFile, float scale /*= 1*/)
{
	SpineRect *pRet = new(std::nothrow) SpineRect();
	pRet->initWithJsonFile(skeletonDataFile, atlasFile, scale);
	pRet->initData(atlasFile);
	pRet->autorelease();

	return pRet;
}

SpineRect* SpineRect::createUseOwnData(const std::string &rSFileName, float scale /*= 1*/)
{
	SpineRect *pRet = new(std::nothrow) SpineRect();
	auto filename = StringUtil::pathWithoutExtension(rSFileName);
	bool isExist = (spSkeletonDataMap.find(filename) != spSkeletonDataMap.end());//判断map里面是否存在 key  skeletonDataFile
	std::string atlasFile = filename + ".atlas";
	std::string jsonFile = filename + ".json";

	pRet->initWithJsonFile(rSFileName, atlasFile, scale);
	pRet->initData(rSFileName);
	pRet->autorelease();

	return pRet;
}

SpineRect* SpineRect::createWithData(spSkeletonData* skeletonData, bool ownsSkeletonData /*= false*/)
{
	SpineRect* node = new(std::nothrow) SpineRect();
	node->initWithData(skeletonData, ownsSkeletonData);
	node->initData(skeletonData->defaultSkin->name);
	node->autorelease();
	return node;
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

myExtension::Rectangle SpineRect::getRectangle(Rect rBoundingBox /*= Rect::ZERO*/)
{
	Vec2 position = this->getPosition();
	Rect box = rBoundingBox.equals(Rect::ZERO) ? this->getBoundingBox() : rBoundingBox;
	float _minx = box.origin.x - position.x;
	float _miny = box.origin.y - position.y;
	float _maxx = box.size.width + _minx;
	float _maxy = box.size.height + _miny;

	myExtension::Rectangle rectangle(_minx, _maxy, _maxx, _miny);
	return rectangle;
}

void SpineRect::drawDebugBox(std::vector<spSlot*>* slots /*= nullptr*/)
{
	this->update(0);//需要先更新骨骼
	Rect rect = this->getBoundingBox(slots);
	myExtension::Rectangle rectangle = this->getRectangle(rect);
	auto drawnode = DrawNode::create();
	Vec2 point[4];
	point[0] = Vec2(rectangle.left, rectangle.bottom);
	point[1] = Vec2(rectangle.left, rectangle.top);
	point[2] = Vec2(rectangle.right, rectangle.top);
	point[3] = Vec2(rectangle.right, rectangle.bottom);
	drawnode->drawPolygon(point, 4, Color4F(1, 0, 0, 0), 1, Color4F(0, 1, 0, 1));

	this->addChild(drawnode);
}

void SpineRect::setPostion(SpineRect::Align alignment,Vec2 pos, std::vector<spSlot*>* slots /*= nullptr*/)
{
	auto finalPos = calculatePosition(alignment, pos, slots);
	this->setPosition(finalPos);
}

cocos2d::Vec2 SpineRect::calculatePosition(SpineRect::Align alignment, const Vec2& pos, std::vector<spSlot*>* slots /*= nullptr*/)
{
	Rect partsBoundingbox = this->getBoundingBox(slots);
	myExtension::Rectangle partsRectangle = this->getRectangle(partsBoundingbox);
	Rect inNodePos = partsRectangle.toRect();
	
	float x = inNodePos.origin.x;
	float y = inNodePos.origin.y;
	float width = inNodePos.size.width;
	float height = inNodePos.size.height;

	
	if (((int)alignment & (int)Align::right) != 0)
		x += width;
	else if (((int)alignment & (int)Align::left) == 0) //
		x += width / 2;

	if (((int)alignment & (int)Align::top) != 0)
		y += height;
	else if (((int)alignment & (int)Align::bottom) == 0) //
		y += height / 2;


	Vec2 _temPoint(x,y);
	auto finalPos = pos - _temPoint;

	return finalPos;
}

void SpineRect::addBoundNodeWithBone(const std::string &boneName, Node* node)
{
	auto bone = this->findBone(boneName);
	Vector<Node*>* nodes = nullptr;
	if (_mapBounds.find(bone) != _mapBounds.end()){
		nodes = _mapBounds.at(bone);
	}
	else{
		nodes = new Vector<Node*>();
		_mapBounds.emplace(bone, nodes);
	}

	nodes->pushBack(node);
	if (node->getParent() == this)return;
	this->addChild(node);

}
void SpineRect::removeBoundNode(Node* node, bool removeFromParent /*= true*/)
{
	for (auto item : _mapBounds)
	{
		auto bone = item.first;
		auto vec = item.second;
		for (auto it = vec->begin(); it != vec->end(); it++)
		{
			if (node == *it){
				if (removeFromParent){
					node->removeFromParent();
				}
				vec->eraseObject(node);
				break;
			}
		}

	}
}

void SpineRect::removeBoundNode(const std::string & nodeName, bool removeFromParent /*= true*/)
{
	for (auto item : _mapBounds)
	{
		auto bone = item.first;
		auto vec = item.second;
		for (auto it = vec->begin(); it != vec->end(); it++)
		{
			auto node = *it;
			if (node->getName() == nodeName){
				if (removeFromParent){
					node->removeFromParent();
				}
				vec->eraseObject(node);
				break;
			}
		}

	}
}

void SpineRect::removeBoundNode(const std::string & boneName, const std::string & nodeName, bool removeFromParent /*= true*/)
{
	auto bone = this->findBone(boneName);
	auto node = findBoneBoundNode(boneName, nodeName);

	if (_mapBounds.find(bone) != _mapBounds.end()){
		Vector<Node*>* nodes = _mapBounds.at(bone);
		if (removeFromParent){
			node->removeFromParent();
		}
		nodes->eraseObject(node);
	}
}

Node* SpineRect::findBoneBoundNode(const std::string & boneName, const std::string & nodeName )
{
	auto bone = this->findBone(boneName);
	Vector<Node*>* nodes = nullptr;
	if (_mapBounds.find(bone) != _mapBounds.end()){
		nodes = _mapBounds.at(bone);

		for (auto it = nodes->begin(); it != nodes->end(); it++)
		{
			auto node = *it;
			if (node->getName() == nodeName){
				return node;
			}
		}
	}

	return nullptr;
}


void SpineRect::update(float deltaTime)
{
	SpineFace::update(deltaTime);
	for (auto item : _mapBounds)
	{
		auto bone = item.first;
		auto vec = item.second;
		for (auto it = vec->begin(); it != vec->end(); it++)
		{
			auto node = *it;
			node->setPosition(Vec2(bone->worldX, bone->worldY));
		}

	}
}
