#pragma once
#include "Vec3.h"
#include <cstdint>

namespace Random
{
    // 乱数生成の初期化
    void Init();

    // 指定範囲内の整数乱数を生成
    int32_t Range(const int32_t min, const int32_t max);

    // 指定範囲内の浮動小数点数乱数を生成
    float RangeF(const float min, const float max);

    // 指定範囲内の2Dベクトル乱数を生成
    Vec2 RangeVec2(const float min, const float max, const bool isSameValue = false);

    // 指定範囲内の3Dベクトル乱数を生成
    Vec3 RangeVec3(const float min, const float max, const bool isSameValue = false);

    // 真偽値を指定確率で生成
    bool RangeB(const float rate);

    // 角度（度数法）を指定範囲内で生成
    float RangeAngle(const float min = 1.f, const float max = 360.f);

    // 弧度法の角度を指定範囲内で生成
    float RangeRadian(const float min = 0.01745f, const float max = 6.28319f);

    // 中心を持つ円内の2Dベクトル乱数を生成
    Vec2 RangeInCircle(const float minRadius, const float maxRadius);
}
