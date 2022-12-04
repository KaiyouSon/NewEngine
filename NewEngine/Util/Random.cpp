#include "Random.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

void Random::Initialize()
{
	srand(time(nullptr));
}

// intŒ^‚Ì—”‚ğæ“¾
int Random::Range(int min, int max)
{
	return rand() % ((max - min) + 1) + min;
}

// floatŒ^‚Ì—”‚ğæ“¾
float Random::RangeF(float min, float max)
{
	double tmp = (min < 0 && max>0) ? (fabs(min) + max) : (fabs(max - min));
	return (double)rand() / RAND_MAX * tmp + min;
}

// Šm—¦‚Åtrue‚ğ•Ô‚·
bool Random::RangeB(float rate)
{
	float num = RangeF(0, 100);
	if (num < rate) return true;
	return false;
}
