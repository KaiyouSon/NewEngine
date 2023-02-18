#include "Util.h"
#include "RenderBase.h"
#include "Camera.h"

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

Vec2 Sign(const Vec2& a)
{
	return { (float)Sign(a.x),(float)Sign(a.y) };
}

Vec3 Sign(const Vec3& a)
{
	return { (float)Sign(a.x),(float)Sign(a.y),(float)Sign(a.z) };
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

float Restore(const float& value, const float& limit, const float& origin)
{
	if (value >= limit)
	{
		return origin + value - limit;
	}
	return value;
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

unsigned long GetNowTime(const TimeUnit& timeUnit)
{
	switch (timeUnit)
	{
	case TimeUnit::MilliSecond:
		return (unsigned long)(GetTickCount64());
		break;
	case TimeUnit::Second:
		return (unsigned long)((GetTickCount64() / 1000) % 60);
		break;
	case TimeUnit::Minute:
		return (unsigned long)((GetTickCount64() / 60000) % 60);
		break;
	default:
		break;
	}

	return -1;
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

// éOäpå`ÇÃñ@ê¸ÇãÅÇﬂÇÈ
Vec3 GetTriangleNormal(const Vec3& p0, const Vec3& p1, const Vec3& p2)
{
	Vec3 v1 = p1 - p0;
	Vec3 v2 = p2 - p0;

	Vec3 normal = Vec3::Cross(v1, v2);

	return normal.Norm();
}