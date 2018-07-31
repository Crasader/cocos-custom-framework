
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
	std::vector<float> temp = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f };
	for (int i = 0; i < temp.size(); i++)
	{
		mUsv[i] = temp[i];
	}

	mFloat = NULL;
}

mySpine::~mySpine()
{
	if (mFloat){
		delete mFloat;
	}
}


void mySpine::myInit()
{
	
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

		switch (slot->attachment->type) {
		case SP_ATTACHMENT_REGION: {
									   spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
									   spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
									   attachmentVertices = getAttachmentVertices(attachment);
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
		if (slotAlphas.find(slot->data->name) != slotAlphas.end()){
			slotAlpha = slotAlphas[slot->data->name];
		}

		color.a *= _skeleton->a * slot->a * 255 * slotAlpha;
		float multiplier = _premultipliedAlpha ? color.a : 255;
		color.r *= _skeleton->r * slot->r * multiplier;
		color.g *= _skeleton->g * slot->g * multiplier;
		color.b *= _skeleton->b * slot->b * multiplier;

		for (int v = 0, w = 0, vn = attachmentVertices->_triangles->vertCount; v < vn; ++v, w += 2) {
			V3F_C4B_T2F* vertex = attachmentVertices->_triangles->verts + v;
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
		if (useSkin.at(slot->data->name))
		{
			texture = useSkin.at(slot->data->name)->getTexture();
		}
		if (texture)
		{
			switch (slot->attachment->type) {
			case SP_ATTACHMENT_REGION: {
										   attachmentVertices->_triangles->verts[0].texCoords.u = 0.0;
										   attachmentVertices->_triangles->verts[0].texCoords.v = 1.0;
										   attachmentVertices->_triangles->verts[1].texCoords.u = 0.0;
										   attachmentVertices->_triangles->verts[1].texCoords.v = 0.0;
										   attachmentVertices->_triangles->verts[2].texCoords.u = 1.0;
										   attachmentVertices->_triangles->verts[2].texCoords.v = 0.0;
										   attachmentVertices->_triangles->verts[3].texCoords.u = 1.0;
										   attachmentVertices->_triangles->verts[3].texCoords.v = 1.0;
			}break;
			case SP_ATTACHMENT_MESH: {
										 float minX = 1.0, maxX = 0.0, minY = 1.0, maxY = 0.0;
										 for (int i = 0; i < attachmentVertices->_triangles->vertCount; i++)
										 {
											 Tex2F t2f = attachmentVertices->_triangles->verts[i].texCoords;
											 if (t2f.u < minX) { minX = t2f.u; }
											 if (t2f.u > maxX) { maxX = t2f.u; }
											 if (t2f.v < minY) { minY = t2f.v; }
											 if (t2f.v > maxY) { maxY = t2f.v; }
										 }
										 float lenx = maxX - minX;
										 float leny = maxY - minY;
										 for (int i = 0; i < attachmentVertices->_triangles->vertCount; i++)
										 {
											 Tex2F t2f = attachmentVertices->_triangles->verts[i].texCoords;
											 attachmentVertices->_triangles->verts[i].texCoords.u -= minX;
											 attachmentVertices->_triangles->verts[i].texCoords.u /= lenx;

											 attachmentVertices->_triangles->verts[i].texCoords.v -= minY;
											 attachmentVertices->_triangles->verts[i].texCoords.v /= leny;
										 }
			}break;
			}

			
		}
		else{
			texture = attachmentVertices->_texture;
		}
		batch->addCommand(renderer, _globalZOrder, texture->getName(), _glProgramState, blendFunc,
			*attachmentVertices->_triangles, transform, transformFlags);
	}
	if (_debugSlots || _debugBones) {
		drawDebug(renderer, transform, transformFlags);
	}
}
// 
// inline void mySpine::drawSkeleton(const Mat4 & transform, uint32_t transformFlags) {
//  	getGLProgramState()->apply(transform);
//  	Color3B nodeColor = getColor();
//  	_skeleton->r = nodeColor.r / (float)255;
//  	_skeleton->g = nodeColor.g / (float)255;
//  	_skeleton->b = nodeColor.b / (float)255;
//  	_skeleton->a = getDisplayedOpacity() / (float)255;
//  
//  	int blendMode = -1;
//  	Color4B color;
//  	const float* uvs = nullptr;
//  	int verticesCount = 0;
//  	const int* triangles = nullptr;
//  	int trianglesCount = 0;
//  	float r = 0, g = 0, b = 0, a = 0;
//  	for (int i = 0, n = _skeleton->slotsCount; i < n; i++) {
//  		spSlot* slot = _skeleton->drawOrder[i];
//  		if (!slot->attachment) continue;
//  		Texture2D *texture = NULL;
//  		if (useSkin.at(slot->data->name))
//  		{
//  			texture = useSkin.at(slot->data->name)->getTexture();
//  		}
//  
//  		switch (slot->attachment->type) {
//  		case SP_ATTACHMENT_REGION: {
//  									   const int quadTriangles[6] = { 0, 1, 2, 2, 3, 0 };
//  									   spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
//  									   spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
//  									   if (texture)
//  									   {
//  										   uvs = mUsv;
//  									   }
//  									   else {
//  										   texture = getTexture(attachment);
//  										   uvs = attachment->uvs;
//  									   }
//  									   verticesCount = 8;
//  									   triangles = quadTriangles;
//  									   trianglesCount = 6;
//  									   r = attachment->r;
//  									   g = attachment->g;
//  									   b = attachment->b;
//  									   a = attachment->a;
//  									   break;
//  		}
//  		case SP_ATTACHMENT_MESH: {
//  									 spMeshAttachment* attachment = (spMeshAttachment*)slot->attachment;
//  									 spMeshAttachment_computeWorldVertices(attachment, slot, _worldVertices);
//  
//  									 if (texture) {
//  										 if (mFloat)
//  										 {
//  											 delete[] mFloat;
//  										 }
//  										 mFloat = new float[attachment->verticesCount];
//  
//  										 float minX = 1.0, maxX = 0.0, minY = 1.0, maxY = 0.0;
//  										 for (int i = 0; i < attachment->verticesCount; i++)
//  										 {
//  											 float n = attachment->uvs[i];
//  											 if (i % 2 == 0)
//  											 {
//  												 if (n < minX) { minX = n; }
//  												 if (n > maxX) { maxX = n; }
//  											 }
//  											 else {
//  												 if (n < minY) { minY = n; }
//  												 if (n > maxY) { maxY = n; }
//  											 }
//  											 mFloat[i] = n;
//  										 }
//  										 float lenx = maxX - minX;
//  										 float leny = maxY - minY;
//  										 for (int i = 0; i < attachment->verticesCount; i++)
//  										 {
//  											 if (i % 2 == 0)
//  											 {
//  												 mFloat[i] -= minX;
//  												 mFloat[i] /= lenx;
//  											 }
//  											 else {
//  												 mFloat[i] -= minY;
//  												 mFloat[i] /= leny;
//  											 }
//  										 }
//  										 uvs = mFloat;
//  									 }
//  									 else {
//  										 texture = getTexture(attachment);
//  										 uvs = attachment->uvs;
//  									 }
//  
//  									 verticesCount = attachment->verticesCount;
//  									 triangles = attachment->triangles;
//  									 trianglesCount = attachment->trianglesCount;
//  									 r = attachment->r;
//  									 g = attachment->g;
//  									 b = attachment->b;
//  									 a = attachment->a;
//  									 break;
//  		}
//  	
//  		default:;
//  		}
//  		if (texture) {
//  			if (slot->data->blendMode != blendMode) {
//  				_batch->flush();
//  				blendMode = slot->data->blendMode;
//  				switch (slot->data->blendMode) {
//  				case SP_BLEND_MODE_ADDITIVE:
//  					GL::blendFunc(_premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA, GL_ONE);
//  					break;
//  				case SP_BLEND_MODE_MULTIPLY:
//  					GL::blendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
//  					break;
//  				case SP_BLEND_MODE_SCREEN:
//  					GL::blendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
//  					break;
//  				default:
//  					GL::blendFunc(_blendFunc.src, _blendFunc.dst);
//  				}
//  			}
//  			float slotAlpha = 1;//自定义的 阿尔法 值
//  			if (slotAlphas.find(slot->data->name) != slotAlphas.end()){
//  				slotAlpha = slotAlphas[slot->data->name];
//  			}
//  			color.a = _skeleton->a * slot->a * a * 255 * slotAlpha;
//  			float multiplier = _premultipliedAlpha ? color.a : 255;
//  			color.r = _skeleton->r * slot->r * r * multiplier;
//  			color.g = _skeleton->g * slot->g * g * multiplier;
//  			color.b = _skeleton->b * slot->b * b * multiplier;
//  			_batch->add(texture, _worldVertices, uvs, verticesCount, triangles, trianglesCount, &color);
//  		}
//  	}
//  	_batch->flush();
//  
// }
void mySpine::setSkinFile(string slot, string file)
{
	useSkin.insert(slot, Sprite::create(file));
}

void mySpine::setSkinFile(string slotName, Texture2D* rTexture2D)
{
	useSkin.insert(slotName, Sprite::createWithTexture(rTexture2D));
}

void mySpine::setSkinFile(string slotName, Sprite* rSprite)
{
	useSkin.insert(slotName, rSprite);
}

void mySpine::cleanCustomSkin()
{
	useSkin.clear();
}

void mySpine::removeCustomSkin(string slotName)
{
	bool has = useSkin.find(slotName) != useSkin.end();
	if (has){
		useSkin.erase(slotName);
	}
}

void mySpine::setSkinAlphas(string slotName, float alpha)
{
	slotAlphas[slotName] = alpha;
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
