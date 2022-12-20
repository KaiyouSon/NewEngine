#include "Util.h"
#include "RenderBase.h"

float Max(const float& a, const float& b)
{
	return a >= b ? a : b;
}

float Min(const float& a, const float& b)
{
	return a <= b ? a : b;
}

int Sign(const float& a)
{
	return a >= 0 ? 1 : -1;
}

float Clamp(const float& value, const float& min, const float& max)
{
	if (value < min)
	{
		return min;
	}
	if (value > max)
	{
		return max;
	}
	return value;
}

float Convergence(const float& value, const float& speed, const float& origin)
{
	float tValue = value;
	if (value == origin)
	{
		return origin;
	}
	else if (value > origin)
	{
		tValue -= fabs(speed);
		return Max(tValue, origin);
	}
	else if (value < origin)
	{
		tValue += fabs(speed);
		return Min(tValue, origin);
	}

	return -1;
}

int GetDight(const int& value)
{
	int tempValue = value;
	int digit = 0;
	while (tempValue != 0)
	{
		tempValue /= 10;
		digit++;
	}
	return digit;
}

Vec2 WorldToScreen(const Vec3& worldPos)
{
	Mat4 viewportMat =
		ConvertViewportMat(*RenderBase::GetInstance()->GetViewport());

	Mat4 finalMat =
		Camera::current.GetViewLookAtMat() *
		Camera::current.GetPerspectiveProjectionMat() *
		viewportMat;

	Vec3 result = Vec3MulMat4(worldPos, finalMat, true);

	return { result.x,result.y };
}