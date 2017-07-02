# ifndef __Direction_H__
# define __Direction_H__


#include "cocos2d.h"
NS_CC_BEGIN
namespace c2d {

	enum class Direction{
		eNone = 0,
		eLeft = -1,
		eRight = 1,
		eTop = 2,
		eBottom = -2,
		eCenter = 3,
	};

} // namespace c2d {

// end of data_structure group
/// @}
NS_CC_END
#endif /* defined(__Align_H__) */
