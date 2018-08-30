
#include "ColorUtils.h"
#include <math.h>       /* isnan, sqrt */
NS_CC_BEGIN

namespace ColorUtils {


	RGBA RGBfromHSV(HSV value)
	{
		double      hh, p, q, t, ff;
		long        i;
		RGBA        out;
		out.a = 1;

		if (value.s <= 0.0) // < is bogus, just shuts up warnings
		{
			if (fpclassify((value.h)) == FP_NAN) // value.h == NAN // may be is "std::isnan"
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

	std::string getHexCodeFromColor4B(const Color4B& color)
	{
		auto hexCode = StringUtils::format("#%02x%02x%02x%02x", color.r, color.g, color.b,color.a);
		return hexCode;
	}

	cocos2d::Color4B getColorFormHexCode(const std::string& rHexCode)
	{
		int r = 255, g = 255, b = 255, a = 255;
		std::string hexCode = rHexCode;
		if (hexCode.at(0) == '#') {
			hexCode = hexCode.erase(0, 1);
		}

		// ... and extract the rgb values.
		std::istringstream(hexCode.substr(0, 2)) >> std::hex >> r;
		std::istringstream(hexCode.substr(2, 2)) >> std::hex >> g;
		std::istringstream(hexCode.substr(4, 2)) >> std::hex >> b;

		if (hexCode.size() == 8){
			std::istringstream(hexCode.substr(6, 2)) >> std::hex >> a;
		}
		return Color4B(r, g, b, a);
	}

} // namespace ColorUtils {


NS_CC_END