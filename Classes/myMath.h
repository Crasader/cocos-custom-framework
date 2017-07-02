#ifndef __MY_MATH_H__
#define __MY_MATH_H__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
#include <string.h>
#endif

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

NS_CC_BEGIN
namespace c2d {

	//È¡Õý¸º
	int sgn(double d);

} // namespace c2d {

// end of data_structure group
/// @}
NS_CC_END
#endif /* defined(__MY_MATH_H__) */
