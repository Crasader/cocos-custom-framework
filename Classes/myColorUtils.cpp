
#include "myColorUtils.h"


NS_CC_BEGIN

namespace c2d {


	RGBA RGBfromHSV(HSV value)
	{
		double      hh, p, q, t, ff;
		long        i;
		RGBA        out;
		out.a = 1;

		if (value.s <= 0.0) // < is bogus, just shuts up warnings
		{
			if (std::isnan(value.h)) // value.h == NAN
			{
				out.r = value.v;
				out.g = value.v;
				out.b = value.v;
				return out;
			}

			// error - should never happen
			out.r = 0.0;
			out.g = 0.0;
			out.b = 0.0;
			return out;
		}

		hh = value.h;
		if (hh >= 360.0) hh = 0.0;
		hh /= 60.0;
		i = (long)hh;
		ff = hh - i;
		p = value.v * (1.0 - value.s);
		q = value.v * (1.0 - (value.s * ff));
		t = value.v * (1.0 - (value.s * (1.0 - ff)));

		switch (i)
		{
		case 0:
			out.r = value.v;
			out.g = t;
			out.b = p;
			break;
		case 1:
			out.r = q;
			out.g = value.v;
			out.b = p;
			break;
		case 2:
			out.r = p;
			out.g = value.v;
			out.b = t;
			break;

		case 3:
			out.r = p;
			out.g = q;
			out.b = value.v;
			break;
		case 4:
			out.r = t;
			out.g = p;
			out.b = value.v;
			break;
		case 5:
		default:
			out.r = value.v;
			out.g = p;
			out.b = q;
			break;
		}
		return out;
	}

	HSV HSVfromRGB(RGBA value)
	{

		HSV         out;
		double      min, max, delta;

		min = value.r < value.g ? value.r : value.g;
		min = min  < value.b ? min : value.b;

		max = value.r > value.g ? value.r : value.g;
		max = max  > value.b ? max : value.b;

		out.v = max;                                // v
		delta = max - min;
		if (max > 0.0)
		{
			out.s = (delta / max);                  // s
		}
		else
		{
			// r = g = b = 0                        // s = 0, v is undefined
			out.s = 0.0;
			out.h = -1;                            // its now undefined (don't know if setting to NAN is a good idea)
			return out;
		}
		if (value.r >= max)                        // > is bogus, just keeps compilor happy
		{
			out.h = (value.g - value.b) / delta;        // between yellow & magenta
		}
		else
		{
			if (value.g >= max)
				out.h = 2.0 + (value.b - value.r) / delta;  // between cyan & yellow
			else
				out.h = 4.0 + (value.r - value.g) / delta;  // between magenta & cyan
		}

		out.h *= 60.0;                              // degrees

		if (out.h < 0.0)
			out.h += 360.0;

		return out;
	}

	cocos2d::Color3B RGBA2Color3B(RGBA rgba)
	{
		return Color3B((GLubyte)(rgba.r * 255.0f), (GLubyte)(rgba.g * 255.0f), (GLubyte)(rgba.b * 255.0f));
	}

} // namespace ColorUtils {


NS_CC_END