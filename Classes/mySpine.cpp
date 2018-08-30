
#include "mySpine.h"
#include <spine/SkeletonRenderer.h>
#include <spine/extension.h>
#include <spine/SkeletonBatch.h>
#include <spine/AttachmentVertices.h>
#include <spine/Cocos2dAttachmentLoader.h>
#include <algorithm>


mySpine::mySpine()
:SkeletonAnimation()
{
	_pTriangles = new TrianglesCommand::Triangles();

}

mySpine::~mySpine()
{

	if (_pTriangles->verts) {
		free(_pTriangles->verts);
	}
	delete _pTriangles;


	useSkin.clear();
	slotAlphas.clear();
	visualSlotAlphas.clear();
	durations.clear();

	for (auto item : useAttachment)
	{
		item.second.clear();
	}

	useAttachment.clear();
}


mySpine * mySpine::create(const std::string& skeletonDataFile, const std::string& atlasFile, float scale/*= 1*/)
{
	mySpine* node = new mySpine();
	node->initWithJsonFile(skeletonDataFile, atlasFile, scale);
	node->autorelease();
	return node;
}


mySpine* mySpine::createWithData(spSkeletonData* skeletonData, bool ownsSkeletonData /*= false*/)
{
	mySpine* node = new mySpine();
	node->initWithData(skeletonData, ownsSkeletonData);
	node->autorelease();

	return node;
}

void mySpine::copyTriangles(cocos2d::TrianglesCommand::Triangles* target, cocos2d::TrianglesCommand::Triangles* source)
{
	if (target->verts) {
		free(target->verts);
		target->verts = NULL;
	}

	target->verts = (V3F_C4B_T2F *)malloc(sizeof(V3F_C4B_T2F)* source->vertCount);
	memcpy(target->verts, source->verts, sizeof(V3F_C4B_T2F)* source->vertCount);

	target->vertCount = source->vertCount;
	target->indexCount = source->indexCount;
	target->indices = source->indices;
}

void mySpine::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformFlags)
{
	SkeletonBatch* batch = SkeletonBatch::getInstance();
	Color3B nodeColor = getColor();
	_skeleton->r = nodeColor.r / (float)255;
	_skeleton->g = nodeColor.g / (float)255;
	_skeleton->b = nodeColor.b / (float)255;
	_skeleton->a = getDisplayedOpacity() / (float)255;

	Color4F color;

	AttachmentVertices* attachmentVertices = nullptr;
	for (int i = 0, n = _skeleton->slotsCount; i < n; ++i) {
		spSlot* slot = _skeleton->drawOrder[i];
		if (!slot->attachment) continue;
		//CCLOG("the attachment -> %s", slot->attachment->name);
		switch (slot->attachment->type) {
		case SP_ATTACHMENT_REGION: {
									   spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
									   spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
									   attachmentVertices = getAttachmentVertices(attachment);
									   copyTriangles(_pTriangles,attachmentVertices->_triangles);
									   color.r = attachment->r;
									   color.g = attachment->g;
									   color.b = attachment->b;
									   color.a = attachment->a;
									   break;
		}
		case SP_ATTACHMENT_MESH: {
									 spMeshAttachment* attachment = (spMeshAttachment*)slot->attachment;
									 spMeshAttachment_computeWorldVertices(attachment, slot, _worldVertices);
									 attachmentVertices = getAttachmentVertices(attachment);
									 copyTriangles(_pTriangles, attachmentVertices->_triangles);
									 color.r = attachment->r;
									 color.g = attachment->g;
									 color.b = attachment->b;
									 color.a = attachment->a;
									 break;
		}
		default:
			continue;
		}



		float slotAlpha = 1;//自定义的 阿尔法 值
		float visualSlotAlpha = 1;
		if (slotAlphas.find(slot->data->name) != slotAlphas.end()){
			slotAlpha = slotAlphas[slot->data->name];
			float _deltaTime = Director::getInstance()->getDeltaTime();
			float duration = durations[slot->data->name];
			visualSlotAlpha = visualSlotAlphas[slot->data->name];

			if (duration > 0){
				if (visualSlotAlpha > slotAlpha){
					visualSlotAlpha = MAX(slotAlpha, visualSlotAlpha - _deltaTime / duration);
				}
				if (visualSlotAlpha < slotAlpha){
					visualSlotAlpha = MIN(slotAlpha, visualSlotAlpha + _deltaTime / duration);
				}
				visualSlotAlphas[slot->data->name] = visualSlotAlpha;
			}
			else{
				visualSlotAlpha = slotAlpha;

			}
			//CCLOG("the [%s] visualSlotAlpha -> %.02f", slot->data->name, slotAlpha);
		}

		color.a *= _skeleton->a * slot->a * 255 * visualSlotAlpha;
		float multiplier = _premultipliedAlpha ? color.a : 255;
		color.r *= _skeleton->r * slot->r * multiplier;
		color.g *= _skeleton->g * slot->g * multiplier;
		color.b *= _skeleton->b * slot->b * multiplier;

		for (int v = 0, w = 0, vn = _pTriangles->vertCount; v < vn; ++v, w += 2) {
			V3F_C4B_T2F* vertex = _pTriangles->verts + v;
			vertex->vertices.x = _worldVertices[w];
			vertex->vertices.y = _worldVertices[w + 1];
			vertex->colors.r = (GLubyte)color.r;
			vertex->colors.g = (GLubyte)color.g;
			vertex->colors.b = (GLubyte)color.b;
			vertex->colors.a = (GLubyte)color.a;
		}

		BlendFunc blendFunc;
		switch (slot->data->blendMode) {
		case SP_BLEND_MODE_ADDITIVE:
			blendFunc.src = _premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA;
			blendFunc.dst = GL_ONE;
			break;
		case SP_BLEND_MODE_MULTIPLY:
			blendFunc.src = GL_DST_COLOR;
			blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
			break;
		case SP_BLEND_MODE_SCREEN:
			blendFunc.src = GL_ONE;
			blendFunc.dst = GL_ONE_MINUS_SRC_COLOR;
			break;
		default:
			blendFunc.src = _premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA;
			blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Texture2D* texture = nullptr;

		auto sprit = useSkin.find(slot->data->name);
		if (sprit != useSkin.end())
		{
			texture = sprit->second->getTexture();
		}

		auto ruseAttachmentslot = useAttachment.find(slot->data->name);
		if (ruseAttachmentslot != useAttachment.end()){
			//slot->attachment
			auto rmap = ruseAttachmentslot->second;
			auto attachmentit = rmap.find(slot->attachment->name);
			if (attachmentit != rmap.end()){
				Sprite* sp = attachmentit->second;
				texture = sp->getTexture();
			}
		}

		if (texture)
		{
			switch (slot->attachment->type) {
			case SP_ATTACHMENT_REGION: {
										   _pTriangles->verts[0].texCoords.u = 0.0;
										   _pTriangles->verts[0].texCoords.v = 1.0;
										   _pTriangles->verts[1].texCoords.u = 0.0;
										   _pTriangles->verts[1].texCoords.v = 0.0;
										   _pTriangles->verts[2].texCoords.u = 1.0;
										   _pTriangles->verts[2].texCoords.v = 0.0;
										   _pTriangles->verts[3].texCoords.u = 1.0;
										   _pTriangles->verts[3].texCoords.v = 1.0;
			}break;
			case SP_ATTACHMENT_MESH: {
										 float minX = 1.0, maxX = 0.0, minY = 1.0, maxY = 0.0;
										 for (int i = 0; i < _pTriangles->vertCount; i++)
										 {
											 Tex2F t2f = _pTriangles->verts[i].texCoords;
											 if (t2f.u < minX) { minX = t2f.u; }
											 if (t2f.u > maxX) { maxX = t2f.u; }
											 if (t2f.v < minY) { minY = t2f.v; }
											 if (t2f.v > maxY) { maxY = t2f.v; }
										 }
										 float lenx = maxX - minX;
										 float leny = maxY - minY;
										 for (int i = 0; i < _pTriangles->vertCount; i++)
										 {
											 Tex2F t2f = _pTriangles->verts[i].texCoords;
											 _pTriangles->verts[i].texCoords.u -= minX;
											 _pTriangles->verts[i].texCoords.u /= lenx;

											 _pTriangles->verts[i].texCoords.v -= minY;
											 _pTriangles->verts[i].texCoords.v /= leny;
										 }
			}break;
			}

			
		}
		else{
			texture = attachmentVertices->_texture;
		}
		batch->addCommand(renderer, _globalZOrder, texture->getName(), _glProgramState, blendFunc,
			*_pTriangles, transform, transformFlags);
	}
	if (_debugSlots || _debugBones) {
		drawDebug(renderer, transform, transformFlags);
	}
}
void mySpine::setSkinFileWithData(const std::string& slot, const std::string& file)
{
	Texture2D *texture = _director->getTextureCache()->addImage(file);
	CCASSERT(texture != nullptr, StringUtils::format("the %s is not exist", file.c_str()).c_str());
	auto sprite = Sprite::createWithTexture(texture);
	useSkin.insert(slot, sprite);
}

void mySpine::setSkinFile(const std::string& slot, const std::string& file)
{
	auto sprite = Sprite::create(file);
	CCASSERT(sprite != nullptr, StringUtils::format("the %s is not exist", file.c_str()).c_str());
	useSkin.insert(slot, sprite);
}

void mySpine::setSkinFile(const std::string& slotName, Texture2D* rTexture2D)
{
	CCASSERT(rTexture2D != nullptr, StringUtils::format("the %s is not exist", rTexture2D->getPath().c_str()).c_str());
	auto sprite = Sprite::createWithTexture(rTexture2D);
	useSkin.insert(slotName, sprite);
}

void mySpine::setSkinFile(const std::string& slotName, Sprite* rSprite)
{

	CCASSERT(rSprite != nullptr, StringUtils::format("the %s is not exist", rSprite->getTexture()->getPath().c_str()).c_str());
	useSkin.insert(slotName, rSprite);
}

void mySpine::cleanCustomSkin()
{
	useSkin.clear();
}

void mySpine::removeCustomSkin(const std::string& slotName)
{
	bool has = useSkin.find(slotName) != useSkin.end();
	if (has){
		useSkin.erase(slotName);
	}
}

void mySpine::setSkinAlphas(const std::string& slotName, float alpha, float duration/* = 0*/)
{
	slotAlphas[slotName] = alpha;
	visualSlotAlphas[slotName] = 1;
	durations[slotName] = duration;
}


int mySpine::getBonesCount()
{
	return _skeleton->bonesCount;
}
spBone** mySpine::getBones()
{
	return _skeleton->bones;
}

int mySpine::getSlotsCount()
{
	return _skeleton->slotsCount;
}

spSlot** mySpine::getSlots()
{
	return _skeleton->slots;
}

void mySpine::setCustomAttachment(const std::string& slotName, const std::string attachmentName, const std::string& filepath)
{
	auto sprite = Sprite::create(filepath);
	if (useAttachment.find(slotName) != useAttachment.end()){
	
		useAttachment.at(slotName).insert(attachmentName, sprite);
	}
	else{
		Map<std::string, Sprite*> rmap;
		rmap.insert(attachmentName, sprite);
		useAttachment.emplace(slotName, rmap);
	}
}


void mySpine::removeSkinAlphas(const std::string& slotName)
{
	if (slotAlphas.find(slotName) != slotAlphas.end()){
		slotAlphas.erase(slotName);
	}
	if (durations.find(slotName) != durations.end()){
		durations.erase(slotName);
	}
	if (visualSlotAlphas.find(slotName) != visualSlotAlphas.end()){
		visualSlotAlphas.erase(slotName);
	}
}

void mySpine::removeCustomAttachment(const std::string& slotName, const std::string attachmentName)
{
	auto it = useAttachment.find(slotName);
	if (it != useAttachment.end()){

		auto attachmentit = it->second.find(attachmentName);
		if (attachmentit != it->second.end()){
			it->second.erase(attachmentit);
		}
	}
	else{
		CCLOG("can't find the [ %s ] attachment in [ %s ].", attachmentName.c_str(), slotName.c_str());
	}
}

void mySpine::logSlots()
{
	for (int i = 0, n = _skeleton->slotsCount; i < n; ++i) {
		spSlot* slot = _skeleton->slots[i];
		CCLOG("the slot name -> %s ", slot->data->name);
	}
}
void mySpine::logAttachments()
{
	std::map<std::string, std::vector<std::string>> spinestruct;

	for (int i = 0, n = _skeleton->slotsCount; i < n; ++i) {
		spSlot* slot = _skeleton->slots[i];
		auto slotName = slot->data->name;
		int slotIndex = spSkeletonData_findSlotIndex(_skeleton->data, slotName);

		std::vector<std::string> strs;

		if (_skeleton->skin) {
			const _Entry* entry = SUB_CAST(_spSkin, _skeleton->skin)->entries;
			while (entry) {
				if (entry->slotIndex == slotIndex){
					strs.push_back(entry->attachment->name);
				}
				entry = entry->next;

			}

			spinestruct[slotName] = strs;
			continue;
		}
		if (_skeleton->data->defaultSkin) {
			const _Entry* entry = SUB_CAST(_spSkin, _skeleton->data->defaultSkin)->entries;
			while (entry) {
				if (entry->slotIndex == slotIndex){
					strs.push_back(entry->attachment->name);
				}
				entry = entry->next;
			}
			spinestruct[slotName] = strs;
			continue;

		}

	}

	CCLOG("logAttachment begin --------------");

	int index = 0;
	CCLOG("{");
	for (auto item : spinestruct)
	{
		auto slotName = item.first;
		auto atts = item.second;
		CCLOG("\"%s\":[", slotName.c_str());
		//CCLOG("\"attachments\":[");
		for (int i = 0, n = atts.size(); i < n; i++)
		{

			if (i == n - 1){
				CCLOG("\"%s\"", atts.at(i).c_str());
			}
			else{
				CCLOG("\"%s\",", atts.at(i).c_str());
			}
		}
		//CCLOG("]");

		if (index == spinestruct.size() - 1){
			CCLOG("]");
		}
		else{
			CCLOG("],");
		}

		index++;
	}
	CCLOG("}");


	CCLOG("logAttachment end --------------");
}
