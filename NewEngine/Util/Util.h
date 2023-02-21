#pragma once
#include "Color.h"
#include "FrameRate.h"
#include "Random.h"
#include "Singleton.h"
#include "Transform.h"
#include "SceneManager.h"
#include "Timer.h"
#include "Easing.h"
#include "BezierCurve.h"
#include <memory>
#include <vector>
#include <string>
#include <list>

static const int maxBoneIndices = 4;

enum class BlendMode
{
	Alpha,
	Add,
	Sub,
	Inv,
	Screen
};

enum class TimeUnit
{
	MilliSecond,
	Second,
	Minute
};

// 比較して大きい方を返す
float Max(const float& a, const float& b);

// 比較して小さい方を返す
float Min(const float& a, const float& b);

// 符号を返す（ -1, 0, 1 ）
int Sign(const float& a);
Vec2 Sign(const Vec2& a);
Vec3 Sign(const Vec3& a);

// 値をMinとMaxの間に制限する関数
float Clamp(const float& value, const float& min = 0, const float& max = 1);

// 値が収束する関数
float Convergence(const float& value, const float& speed, const float& origin = 0);

// 値を超えたら戻す
float Restore(const float& value, const float& limit, const float& origin = 0);

// 桁数を取得
int GetDight(const int& value);

// 現在時間を返す関数
unsigned long GetNowTime(const TimeUnit& timeUnit = TimeUnit::MilliSecond);

// ワールド座標をスクリーン座標に変換する
Vec2 WorldToScreen(const Vec3& worldPos);

// 三角形の法線を求める
Vec3 GetTriangleNormal(const Vec3& p0, const Vec3& p1, const Vec3& p2);