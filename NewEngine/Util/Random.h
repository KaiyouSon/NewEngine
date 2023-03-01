#pragma once
#include "Vec3.h"

class Random
{
public:
	static void Init();

	// int�^�̗������擾
	static int Range(const int& min, const int& max);

	// float�^�̗������擾
	static float RangeF(const float& min, const float& max);

	// Vec2�^�̗������擾
	static Vec2 RangeVec2(const float& min, const float& max, const bool& isSameValue = false);

	// Vec3�^�̗������擾
	static Vec3 RangeVec3(const float& min, const float& max, const bool& isSameValue = false);

	// �m����true��Ԃ�
	static bool RangeB(const float& rate);

	// �����_���̊p�x��Ԃ�
	static float RangeAngle(const float& min = 1.f, const float& max = 360.f);

	// �����_���̃��W�A����Ԃ�
	static float RangeRadian(const float& min = 0.01745f, const float& max = 6.28319f);

	// �����_���ŉ~���̓_��Ԃ�
	static Vec2 RangeInCircle(const float& minRadius, const float& maxRadius);
};