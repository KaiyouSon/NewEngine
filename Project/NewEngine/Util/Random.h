#pragma once
#include "Vec3.h"
#include <cstdint>

namespace Random
{
	void Init();

	// uint32_t�^�̗������擾
	int32_t Range(const int32_t min, const int32_t max);

	// float�^�̗������擾
	float RangeF(const float min, const float max);

	// Vec2�^�̗������擾
	Vec2 RangeVec2(const float min, const float max, const bool isSameValue = false);

	// Vec3�^�̗������擾
	Vec3 RangeVec3(const float min, const float max, const bool isSameValue = false);

	// �m����true��Ԃ�
	bool RangeB(const float rate);

	// �����_���̊p�x��Ԃ�
	float RangeAngle(const float min = 1.f, const float max = 360.f);

	// �����_���̃��W�A����Ԃ�
	float RangeRadian(const float min = 0.01745f, const float max = 6.28319f);

	// �����_���ŉ~���̓_��Ԃ�
	Vec2 RangeInCircle(const float minRadius, const float maxRadius);
}