#pragma once
#include "Vec2.h"

struct RectAngle
{
	float left;
	float right;
	float top;
	float bottom;

	RectAngle();
	RectAngle(const float left, const float right, const float top, const float bottom);
	RectAngle(const Vec2 leftTop, const Vec2 rightBottom);
};
