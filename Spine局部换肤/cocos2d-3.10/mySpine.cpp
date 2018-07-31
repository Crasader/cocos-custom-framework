
#include "mySpine.h"

mySpine::mySpine(const std::string & skeletonDataFile, const std::string & atlasFile, float scale/*= 1*/)
:SkeletonAnimation(skeletonDataFile, atlasFile, scale)
{
	myInit();
}

mySpine::mySpine(spSkeletonData* skeletonData, bool ownsSkeletonData /*= false*/)
: SkeletonAnimation(skeletonData, ownsSkeletonData)
{
	myInit();
}


void mySpine::myInit()
{
	std::vector<float> temp = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f };
	for (int i = 0; i < temp.size(); i++)
	{
		mUsv[i] = temp[i];
	}

	mFloat = NULL;
	initialize();
}

mySpine * mySpine::create(const std::string& skeletonDataFile, const std::string& atlasFile, float scale/*= 1*/)
{
	mySpine* node = new mySpine(skeletonDataFile, atlasFile, scale);
	node->autorelease();
	return node;
}


mySpine* mySpine::createWithData(spSkeletonData* skeletonData, bool ownsSkeletonData /*= false*/)
{
	mySpine* node = new mySpine(skeletonData, ownsSkeletonData);
	node->autorelease();
	return node;
}

inline void mySpine::drawSkeleton(const Mat4 & transform, uint32_t transformFlags) {
	getGLProgramState()->apply(transform);
	Color3B nodeColor = getColor();
	_skeleton->r = nodeColor.r / (float)255;
	_skeleton->g = nodeColor.g / (float)255;
	_skeleton->b = nodeColor.b / (float)255;
	_skeleton->a = getDisplayedOpacity() / (float)255;

	int blendMode = -1;
	Color4B color;
	const float* uvs = nullptr;
	int verticesCount = 0;
	const int* triangles = nullptr;
	int trianglesCount = 0;
	float r = 0, g = 0, b = 0, a = 0;
	for (int i = 0, n = _skeleton->slotsCount; i < n; i++) {
		spSlot* slot = _skeleton->drawOrder[i];
		if (!slot->attachment) continue;
		Texture2D *texture = NULL;
		if (useSkin.at(slot->data->name))
		{
			texture = useSkin.at(slot->data->name)->getTexture();
		}

		switch (slot->attachment->type) {
		case SP_ATTACHMENT_REGION: {
									   const int quadTriangles[6] = { 0, 1, 2, 2, 3, 0 };
									   spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
									   spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
									   if (texture)
									   {
										   uvs = mUsv;
									   }
									   else {
										   texture = getTexture(attachment);
										   uvs = attachment->uvs;
									   }
									   verticesCount = 8;
									   triangles = quadTriangles;
									   trianglesCount = 6;
									   r = attachment->r;
									   g = attachment->g;
									   b = attachment->b;
									   a = attachment->a;
									   break;
		}
		case SP_ATTACHMENT_MESH: {
									 spMeshAttachment* attachment = (spMeshAttachment*)slot->attachment;
									 spMeshAttachment_computeWorldVertices(attachment, slot, _worldVertices);

									 if (texture) {
										 if (mFloat)
										 {
											 delete[] mFloat;
										 }
										 mFloat = new float[attachment->verticesCount];

										 float minX = 1.0, maxX = 0.0, minY = 1.0, maxY = 0.0;
										 for (int i = 0; i < attachment->verticesCount; i++)
										 {
											 float n = attachment->uvs[i];
											 if (i % 2 == 0)
											 {
												 if (n < minX) { minX = n; }
												 if (n > maxX) { maxX = n; }
											 }
											 else {
												 if (n < minY) { minY = n; }
												 if (n > maxY) { maxY = n; }
											 }
											 mFloat[i] = n;
										 }
										 float lenx = maxX - minX;
										 float leny = maxY - minY;
										 for (int i = 0; i < attachment->verticesCount; i++)
										 {
											 if (i % 2 == 0)
											 {
												 mFloat[i] -= minX;
												 mFloat[i] /= lenx;
											 }
											 else {
												 mFloat[i] -= minY;
												 mFloat[i] /= leny;
											 }
										 }
										 uvs = mFloat;
									 }
									 else {
										 texture = getTexture(attachment);
										 uvs = attachment->uvs;
									 }

									 verticesCount = attachment->verticesCount;
									 triangles = attachment->triangles;
									 trianglesCount = attachment->trianglesCount;
									 r = attachment->r;
									 g = attachment->g;
									 b = attachment->b;
									 a = attachment->a;
									 break;
		}
		case SP_ATTACHMENT_SKINNED_MESH: {
											 spSkinnedMeshAttachment* attachment = (spSkinnedMeshAttachment*)slot->attachment;
											 spSkinnedMeshAttachment_computeWorldVertices(attachment, slot, _worldVertices);

											 if (!texture)
											 {
												 texture = getTexture(attachment);
												 uvs = attachment->uvs;
											 }
											 else {
												 if (mFloat)
												 {
													 delete[] mFloat;
												 }
												 mFloat = new float[attachment->uvsCount];

												 float minX = 1.0, maxX = 0.0, minY = 1.0, maxY = 0.0;
												 for (int i = 0; i < attachment->uvsCount; i++)
												 {
													 float n = attachment->uvs[i];
													 if (i % 2 == 0)
													 {
														 if (n < minX) { minX = n; }
														 if (n > maxX) { maxX = n; }
													 }
													 else {
														 if (n < minY) { minY = n; }
														 if (n > maxY) { maxY = n; }
													 }
													 mFloat[i] = n;
												 }
												 float lenx = maxX - minX;
												 float leny = maxY - minY;
												 for (int i = 0; i < attachment->uvsCount; i++)
												 {
													 if (i % 2 == 0)
													 {
														 mFloat[i] -= minX;
														 mFloat[i] /= lenx;
													 }
													 else {
														 mFloat[i] -= minY;
														 mFloat[i] /= leny;
													 }
												 }
												 uvs = mFloat;
											 }

											 verticesCount = attachment->uvsCount;
											 triangles = attachment->triangles;
											 trianglesCount = attachment->trianglesCount;

											 r = attachment->r;
											 g = attachment->g;
											 b = attachment->b;
											 a = attachment->a;

											 break;
		}
		default:;
		}
		if (texture) {
			if (slot->data->blendMode != blendMode) {
				_batch->flush();
				blendMode = slot->data->blendMode;
				switch (slot->data->blendMode) {
				case SP_BLEND_MODE_ADDITIVE:
					GL::blendFunc(_premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA, GL_ONE);
					break;
				case SP_BLEND_MODE_MULTIPLY:
					GL::blendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
					break;
				case SP_BLEND_MODE_SCREEN:
					GL::blendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
					break;
				default:
					GL::blendFunc(_blendFunc.src, _blendFunc.dst);
				}
			}
			float slotAlpha = 1;//自定义的 阿尔法 值
			if (slotAlphas.find(slot->data->name) != slotAlphas.end()){
				slotAlpha = slotAlphas[slot->data->name];
			}
			color.a = _skeleton->a * slot->a * a * 255 * slotAlpha;
			float multiplier = _premultipliedAlpha ? color.a : 255;
			color.r = _skeleton->r * slot->r * r * multiplier;
			color.g = _skeleton->g * slot->g * g * multiplier;
			color.b = _skeleton->b * slot->b * b * multiplier;
			_batch->add(texture, _worldVertices, uvs, verticesCount, triangles, trianglesCount, &color);
		}
	}
	_batch->flush();

	if (_debugSlots || _debugBones) {
		Director* director = Director::getInstance();
		director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
		director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

		if (_debugSlots) {
			// Slots.
			DrawPrimitives::setDrawColor4B(0, 0, 255, 255);
			glLineWidth(1);
			Vec2 points[4];
			V3F_C4B_T2F_Quad quad;
			for (int i = 0, n = _skeleton->slotsCount; i < n; i++) {
				spSlot* slot = _skeleton->drawOrder[i];
				if (!slot->attachment || slot->attachment->type != SP_ATTACHMENT_REGION) continue;
				spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
				spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
				points[0] = Vec2(_worldVertices[0], _worldVertices[1]);
				points[1] = Vec2(_worldVertices[2], _worldVertices[3]);
				points[2] = Vec2(_worldVertices[4], _worldVertices[5]);
				points[3] = Vec2(_worldVertices[6], _worldVertices[7]);
				DrawPrimitives::drawPoly(points, 4, true);
			}
		}
		if (_debugBones) {
			// Bone lengths.
			glLineWidth(2);
			DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
			for (int i = 0, n = _skeleton->bonesCount; i < n; i++) {
				spBone *bone = _skeleton->bones[i];
				float x = bone->data->length * bone->m00 + bone->worldX;
				float y = bone->data->length * bone->m10 + bone->worldY;
				DrawPrimitives::drawLine(Vec2(bone->worldX, bone->worldY), Vec2(x, y));
			}
			// Bone origins.
			DrawPrimitives::setPointSize(4);
			DrawPrimitives::setDrawColor4B(0, 0, 255, 255); // Root bone is blue.
			for (int i = 0, n = _skeleton->bonesCount; i < n; i++) {
				spBone *bone = _skeleton->bones[i];
				DrawPrimitives::drawPoint(Vec2(bone->worldX, bone->worldY));
				if (i == 0) DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
			}
		}
		director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	}
}
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
