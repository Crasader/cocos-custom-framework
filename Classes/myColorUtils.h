#ifndef __COLORUTILS_H__
#define __COLORUTILS_H__

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
NS_CC_BEGIN
namespace c2d {
	typedef struct
	{
		double r;       // percent 0~1
		double g;       // percent 0~1
		double b;       // percent 0~1
		double a;       // percent 0~1
	} RGBA;

	typedef struct
	{
		double h;       // angle in degrees 0~360
		double s;       // percent 0~1 //
		double v;       // percent 0~1
	} HSV;


	RGBA RGBfromHSV(HSV value);
	
	HSV HSVfromRGB(RGBA value);

	Color3B RGBA2Color3B(RGBA rgba);
} // namespace c2d {

// end of data_structure group
/// @}
NS_CC_END
#endif /* defined(__COLORUTILS_H__) */
