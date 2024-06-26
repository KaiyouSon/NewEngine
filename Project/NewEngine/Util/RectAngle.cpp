#include "RectAngle.h"

RectAngle::RectAngle() :
	left(-1), right(1), top(1), bottom(-1)
{
}

RectAngle::RectAngle(const float left, const float right, const float top, const float bottom) :
	left(left), right(right), top(top), bottom(bottom)
{
}

RectAngle::RectAngle(const Vec2 leftTop, const Vec2 rightBottom) :
	left(leftTop.x), right(rightBottom.x), top(leftTop.y), bottom(rightBottom.y)
{
}

Vec2 RectAngle::GetLeftTop()
{
	return { left, top };
}

Vec2 RectAngle::GetRightBottom()
{
	return Vec2(right, bottom);
}
