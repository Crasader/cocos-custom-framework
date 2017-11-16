# ifndef __Align_H__
# define __Align_H__

#include "cocos2d.h"
NS_CC_BEGIN
namespace c2d {

	enum class Align{
		center = 1 << 0,
		top = 1 << 1,
		bottom = 1 << 2,
		left = 1 << 3,
		right = 1 << 4,

		topLeft = top | left,
		topRight = top | right,
		bottomLeft = bottom | left,
		bottomRight = bottom | right,
	};

} // namespace c2d {

// end of data_structure group
/// @}
NS_CC_END
#endif /* defined(__Align_H__) */
