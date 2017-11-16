
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

	std::vector<int> getRandomWithOutRepetition(int total, int num)
	{
		std::vector<int> input;
		for (int i = 0; i < total; i++) {
			input.push_back(i);
		}

		vector<int> output;
		for (int i = 0; i < num;i++)
		{
			vector<int>::iterator iter = input.begin();
			int index = random(0, (int)input.size()-1);

			iter = iter + index;
			output.push_back(*iter);
			input.erase(iter);
		}

		return output;
	}



} // namespace c2d {


NS_CC_END