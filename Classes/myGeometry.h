#ifndef __MYEXTENSION_H__
#define __MYEXTENSION_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "platform/CCPlatformMacros.h"
#include "base/ccMacros.h"
#include "math/CCMath.h"

NS_CC_BEGIN
namespace c2d {

	//Geometry 几何
	class Rectangle
	{
	public:
		float left;
		float top;
		float right;
		float bottom;

	public:
		Rectangle();
		Rectangle(float left, float top, float right, float bottom);
		Rectangle(const Rectangle& other);
		Rectangle& operator= (const Rectangle& other);
		

		void setRect(float left, float top, float right, float bottom);
		Rect toRect();
		std::string toString();
	};

} // namespace myExtension {

NS_CC_END

#endif /* defined(__MYEXTENSION_H__) */
