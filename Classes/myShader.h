# ifndef __utilShader__
# define __utilShader__
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
namespace myShader {

	cocos2d::GLProgramState* getStrokeProgramState(float outlineSize, cocos2d::Color3B outlineColor, cocos2d::Size textureSize, cocos2d::Color3B foregroundColor = cocos2d::Color3B::WHITE);

} // namespace myShader {

// end of data_structure group
/// @}
NS_CC_END
#endif /* defined(__utilShader__) */
