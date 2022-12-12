#include "Util.h"

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
