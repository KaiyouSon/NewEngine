#pragma once
#include "Vec3.h"
class Random
{
public:
	static void Initialize();
	static int Range(const int& min, const int& max);			// int�^�̗������擾
	static float RangeF(const float& min, const float& max);	// float�^�̗������擾
	static bool RangeB(const float& rate);						// �m����true��Ԃ�
};