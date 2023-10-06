#include "Random.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <random>

void Random::Init()
{
	srand((uint32_t)time(nullptr));
}

int32_t Random::Range(const int32_t min, const int32_t max)
{
	return rand() % ((max - min) + 1) + min;
}

float Random::RangeF(const float min, const float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(min, max);
	return float(dist(gen));
}

Vec2 Random::RangeVec2(const float min, const float max, const bool isSameValue)
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

Vec3 Random::RangeVec3(const float min, const float max, const bool isSameValue)
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

bool Random::RangeB(const float rate)
{
	float num = RangeF(0, 100);
	if (num < rate) return true;
	return false;
}

float Random::RangeAngle(const float min, const float max)
{
	return RangeF(min, max);
}

float Random::RangeRadian(const float min, const float max)
{
	return RangeF(min, max);
}

Vec2 Random::RangeInCircle(const float minRadius, const float maxRadius)
{
	const float randomRadian = RangeRadian();
	const float radius = RangeF(minRadius, maxRadius);
	return { cosf(randomRadian) * radius,sinf(randomRadian) * radius };
}
