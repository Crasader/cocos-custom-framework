# ifndef __C2D_H__
# define __C2D_H__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>

#include <vector>
#include<map>
#include <iostream>
#include<string>
#include<cstring>

#include "Align.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

using namespace std;
using namespace spine;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;
using namespace cocostudio::timeline;

NS_CC_BEGIN
namespace c2d {
		
	
	 	 void touchMove(Widget * node);
	 	 bool adaptScreen(Node * node, c2d::Align align);
	 	 bool adaptScreen(Node * node);
	 	 void adaptScene(Node* sceneLayer);
	 	//�ѽڵ���ΪͼƬ�������
	 	 bool saveNodeAsPng(Node* node, std::function<void(RenderTexture*, const std::string&)> callback = nullptr);
	 	/// <summary>
	 	/// Intersections the specified node1.
	 	/// node1 �� node2 �Ƿ��ཻ
	 	 bool intersection(Node* node1, Node*node2);
	 	
	 	 void drawDebug(Node* root, Color4F color = Color4F(0, 1, 0, 1));
	 	 void drawDebugAll(Node* root, Color4F color = Color4F(0, 1, 0, 1));
	 	 void drawDebugSpine(Node* root, Color4F color = Color4F(0, 1, 0, 1));
	 	 DrawNode* getDrawNode(Rect rect, Color4F color = Color4F(0, 1, 0, 1));
	 	 void setCameraMask(Node* root, Node* node);
	 	 bool is_element_in_vector(vector<int> v, int element);
	 	 void setSafeAnchor(Node* node,float anchorX, float anchorY);
		
	 	 Vec2 GetDistance(Vec2 npoint1, Vec2 npoint2);
	 	 void SetRandomSeed();
	 	 int getRand(int nstart, int nend);
	 	 float getRandFloat(float nstart, float nend);
	 	 bool getRandomBool();
	 	 Point getRectCenter(Rect rect);
	 
	 	 Rect getWorldBoundingBox(Node *node);
	 	 Vec2 getWorldPoint(Node * node);
	 	 Vec2 getInNodePoint(Node *ncurparentnode, Vec2 npoint, Node* ndesnode);
	 
	 	 bool safeReleaseRef(Ref *pRef);
	 
	 	 RenderTexture* createRenderTexture(Size size);
	 	 Image* createImageFromSprite(Sprite *pSpr);
	 	 Sprite* createSprite(Sprite* rModel, Sprite* fore);
	 
	 	//ʹ�õѿ�������ϵ
	 	 Color4B getColor4B(Image* rImage, int x, int y);
	 	 Color4B getColor4B(Image* rImage, Point rPoint);
	 	
	 	 bool isRenderTextureClearInRect(Image* image, Rect rext);// ʹ�õѿ�������ϵ ĳ�������Ƿ���ȫ����  
	 	 bool isRenderTextureClear(Image* image);//�Ƿ���ȫ����  
	 
	 	 std::vector<Rect> splitRect(Size root,int rows,int cols);
	 	
	 	 float getDurationFromSpeed(Node* self, Node* target, float speed);
	 	 float getDurationFromSpeed(const Point& self, const Point& target, float speed);
	 
	 	 long getCurrentTime();

} // namespace c2d {

// end of data_structure group
/// @}
NS_CC_END
#endif /* defined(__C2D_H__) */
