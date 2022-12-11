#pragma once
#include "Vec3.h"

class Random
{
public:
	static void Init();

	static int Range(const int& min, const int& max);			// int�^�̗������擾
	static float RangeF(const float& min, const float& max);	// float�^�̗������擾
	static Vec2 RangeVec2(const int& min, const int& max, const bool& isSameValue = false);		// Vec2�^�̗������擾
	static Vec3 RangeVec3(const int& min, const int& max, const bool& isSameValue = false);		// Vec3�^�̗������擾
	static bool RangeB(const float& rate);						// �m����true��Ԃ�
	static float RangeAngle(const float& min = 1.f, const float& max = 360.f);				// �����_���̊p�x��Ԃ�
	static float RangeRadian(const float& min = 0.01745f, const float& max = 6.28319f);		// �����_���̃��W�A����Ԃ�
	static Vec2 RangeInCircle(const float& minRadius, const float& maxRadius);	// �����_���ŉ~���̓_��Ԃ�
};