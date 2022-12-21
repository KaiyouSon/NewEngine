#include "Random.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

void Random::Init()
{
	srand(time(nullptr));
}

int Random::Range(const int& min, const int& max)
{
	return rand() % ((max - min) + 1) + min;
}

float Random::RangeF(const float& min, const float& max)
{
	double tmp = (min < 0 && max>0) ? (fabs(min) + max) : (fabs(max - min));
	return (float)rand() / RAND_MAX * tmp + min;
}

Vec2 Random::RangeVec2(const int& min, const int& max, const bool& isSameValue)
{
	if (isSameValue == true)
	{
		const float randomValue = RangeF(min, max);
		return { randomValue,randomValue };
	}
	else
	{
		return { RangeF(min, max),RangeF(min, max) };
	}
}

Vec3 Random::RangeVec3(const int& min, const int& max, const bool& isSameValue)
{
	if (isSameValue == true)
	{
		const float randomValue = RangeF(min, max);
		return { randomValue,randomValue,randomValue };
	}
	else
	{
		return { RangeF(min, max),RangeF(min, max),RangeF(min, max) };
	}
}

bool Random::RangeB(const float& rate)
{
	float num = RangeF(0, 100);
	if (num < rate) return true;
	return false;
}

float Random::RangeAngle(const float& min, const float& max)
{
	return RangeF(min, max);
}

float Random::RangeRadian(const float& min, const float& max)
{
	return RangeF(min, max);
}

Vec2 Random::RangeInCircle(const float& minRadius, const float& maxRadius)
{
	const float randomRadian = RangeRadian();
	const float radius = RangeF(minRadius, maxRadius);
	return { cosf(randomRadian) * radius,sinf(randomRadian) * radius };
}