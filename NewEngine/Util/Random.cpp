#include "Random.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

void Random::Initialize()
{
	srand(time(nullptr));
}

// int�^�̗������擾
int Random::Range(const int& min, const int& max)
{
	return rand() % ((max - min) + 1) + min;
}

// float�^�̗������擾
float Random::RangeF(const float& min, const float& max)
{
	double tmp = (min < 0 && max>0) ? (fabs(min) + max) : (fabs(max - min));
	return (double)rand() / RAND_MAX * tmp + min;
}

// �m����true��Ԃ�
bool Random::RangeB(const float& rate)
{
	float num = RangeF(0, 100);
	if (num < rate) return true;
	return false;
}
