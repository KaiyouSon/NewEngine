#pragma once
#include "Color.h"
#include "FrameRate.h"
#include "Random.h"
#include "Singleton.h"
#include "Transform.h"
#include "Timer.h"
#include "Easing.h"
#include "BezierCurve.h"
#include "Enum.h"
#include "Rect.h"
#include <functional>

static const int maxBoneIndices = 4;

// 比較して大きい方を返す
template<typename T>
T Max(const T a, const T b)
{
	return a >= b ? a : b;
}

// 比較して小さい方を返す
template<typename T>
T Min(const T a, const T b)
{
	return a <= b ? a : b;
}

// 符号を返す（ -1, 0, 1 ）
uint32_t Sign(const float a);
Vec2 Sign(const Vec2 a);
Vec3 Sign(const Vec3 a);

// 値をMinとMaxの間に制限する関数
template<typename T>
T Clamp(const T value, const T min = 0, const T max = 1)
{
	if (value < min)
	{
		return min;
	}
	if (value > max)
	{
		return max;
	}
	return value;
}

// 値が収束する関数
template<typename T>
T Convergence(const T value, const T speed, const T origin = 0)
{
	float temp = value;
	if (value == origin)
	{
		return origin;
	}
	else if (value > origin)
	{
		temp -= fabs(speed);
		return Max(temp, origin);
	}
	else if (value < origin)
	{
		temp += fabs(speed);
		return Min(temp, origin);
	}

	return -1;
}

// 値を超えたら戻す
template<typename T>
T Restore(const T value, const T limit, const T origin = 0)
{
	if (value >= limit)
	{
		return origin + value - limit;
	}
	return value;
}

// 桁数を取得
uint32_t GetDight(const uint32_t value);

// 現在時間を返す関数
unsigned long GetNowTime(const TimeUnit timeUnit = TimeUnit::MilliSecond);

// ワールド座標をスクリーン座標に変換する
Vec2 WorldToScreen(const Vec3 worldPos);

// 三角形の法線を求める
Vec3 GetTriangleNormal(const Vec3 p0, const Vec3 p1, const Vec3 p2);

// Debugビルドのみ実行する
void ProcessAtDebugBulid(std::function<void()> lambdaFunc);

// Releaseビルド身の実行する
void ProcessAtReleaseBulid(std::function<void()> lambdaFunc);

// 出力ウィンドウに出力する
void OutputDebugLog(const char* fmt...);