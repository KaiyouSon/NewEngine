#pragma once
#include "Vec3.h"
class Random
{
public:
	static void Initialize();
	static int Range(int min, int max);			// int�^�̗������擾
	static float RangeF(float min, float max);	// float�^�̗������擾
	static bool RangeB(float rate); // �m����true��Ԃ�
};