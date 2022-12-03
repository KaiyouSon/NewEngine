#include "Util.h"

float Util::Max(float a, float b)
{
	if (a > b)return a;
	return b;
}

float Util::Min(float a, float b)
{
	if (a < b)return a;
	return b;
}

int Util::Sign(float a)
{
	if (a > 0)return 1;
	if (a < 0)return -1;
	return 0;
}