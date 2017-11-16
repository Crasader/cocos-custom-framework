#ifndef __MY_MATH_H__
#define __MY_MATH_H__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
#include <string.h>
#endif

#include "cocos2d.h"
#include <thread>
#include <vector>
#include <map>
#include <functional>
#include <mutex>
#include <stdarg.h>
#include <string>
#include <functional>
#include <sstream>
#include "base/CCRef.h"
#include <algorithm>
using namespace std;
using namespace cocos2d;
using namespace cocos2d::experimental;

NS_CC_BEGIN
namespace c2d {

	//取正负
	int sgn(double d);

	//不重复随机数 total 总数，num 随机获取的个数
	std::vector<int> getRandomWithOutRepetition(int total,int num);


} // namespace c2d {

// end of data_structure group
/// @}
NS_CC_END
#endif /* defined(__MY_MATH_H__) */
