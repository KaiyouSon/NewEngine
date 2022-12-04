#include "Util.h"

float Max(float a, float b)
{
	if (a > b)return a;
	return b;
}

float Min(float a, float b)
{
	if (a < b)return a;
	return b;
}

int Sign(float a)
{
	if (a > 0)return 1;
	if (a < 0)return -1;
	return 0;
}