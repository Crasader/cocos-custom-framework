
#include "myMath.h"

#include <stdlib.h>
#include <stdio.h>

NS_CC_BEGIN

namespace c2d {

	int sgn(double d)
	{
		if (d < 0){
			return -1;
		}
		else if (d == 0){
			return 0;
		}else{
			return 1;
		}
	}
} // namespace c2d {


NS_CC_END