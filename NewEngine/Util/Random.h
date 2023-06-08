#pragma once
#include "Vec3.h"
#include <cstdint>

class Random
{
public:
	static void Init();

	// uint32_t型の乱数を取得
	static uint32_t Range(const uint32_t min, const uint32_t max);

	// float型の乱数を取得
	static float RangeF(const float min, const float max);

	// Vec2型の乱数を取得
	static Vec2 RangeVec2(const float min, const float max, const bool isSameValue = false);

	// Vec3型の乱数を取得
	static Vec3 RangeVec3(const float min, const float max, const bool isSameValue = false);

	// 確率でtrueを返す
	static bool RangeB(const float rate);

	// ランダムの角度を返す
	static float RangeAngle(const float min = 1.f, const float max = 360.f);

	// ランダムのラジアンを返す
	static float RangeRadian(const float min = 0.01745f, const float max = 6.28319f);

	// ランダムで円内の点を返す
	static Vec2 RangeInCircle(const float minRadius, const float maxRadius);
};