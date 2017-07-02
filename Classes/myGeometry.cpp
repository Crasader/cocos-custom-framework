#include "myGeometry.h"

NS_CC_BEGIN
namespace c2d {

	Rectangle::Rectangle(void)
	{
		setRect(0.0f, 0.0f, 0.0f, 0.0f);
	}

	Rectangle::Rectangle(float left, float top, float right, float bottom)
	{
		setRect(left, top, right, bottom);
	}

	Rectangle::Rectangle(const Rectangle& other)
	{
		setRect(other.left, other.top, other.right, other.bottom);
	}

	Rectangle& Rectangle::operator=(const Rectangle& other)
	{
		setRect(other.left, other.top, other.right, other.bottom);
		return *this;
	}

	void Rectangle::setRect(float left, float top, float right, float bottom)
	{
		this->left = left;
		this->right = right;
		this->top = top;
		this->bottom = bottom;
	}

	std::string Rectangle::toString()
	{
		return StringUtils::format("Rectangle [%f,%f,%f,%f]", left, top, right, bottom);
	}

	Rect Rectangle::toRect()
	{
		Rect rect(left, bottom, right - left, top - bottom);
		return rect;
	}


}// namespace myExtension {


NS_CC_END