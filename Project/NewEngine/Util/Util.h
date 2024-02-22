#pragma once
#include "MathUtil.h"
#include "Color.h"
#include "FrameRate.h"
#include "Random.h"
#include "Singleton.h"
#include "Transform.h"
#include "Timer.h"
#include "Easing.h"
#include "BezierCurve.h"
#include "RectAngle.h"
#include "Frustum.h"
#include "NewEngineDefine.h"
#include "NewEngineEnum.h"
#include "NewEngineSetting.h"
#include "NewEngineConstant.h"
#include "StandardLib.h"
#include <functional>

static const int maxBoneIndices = 4;


// 2つの値の最大値を返す
template<typename T>
T Max(const T a, const T b)
{
	return a >= b ? a : b;
}

// 2つの値の最小値を返す
template<typename T>
T Min(const T a, const T b)
{
	return a <= b ? a : b;
}

// 浮動小数点数の符号を取得する（-1, 0, 1）
uint32_t Sign(const float a);
Vec2 Sign(const Vec2 a);
Vec3 Sign(const Vec3 a);

// 値を指定範囲内にクランプする
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

// 値を指定された速度で収束させる
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
		temp -= std::fabs(speed);
		return Max(temp, origin);
	}
	else if (value < origin)
	{
		temp += std::fabs(speed);
		return Min(temp, origin);
	}

	return -1;
}

// 値を指定された制限内で復元する
template<typename T>
T Restore(const T value, const T limit, const T origin = 0)
{
	if (value >= limit)
	{
		return origin + value - limit;
	}
	return value;
}

// 整数の桁数を取得する
uint32_t GetDigit(const uint32_t value);

// 現在の時間を取得する
uint64_t GetNowTime(const TimeUnit timeUnit = TimeUnit::MilliSecond);

// ワールド座標をスクリーン座標に変換する
Vec2 WorldToScreen(const Vec3 worldPos);

// スクリーン座標をワールド座標に変換する
Vec3 ScreenToWorld(const Vec2 screenPos);

// 3つの点から三角形の法線ベクトルを計算する
Vec3 GetTriangleNormal(const Vec3 p0, const Vec3 p1, const Vec3 p2);

// デバッグビルドでのみ実行される処理
void ProcessAtDebugBuild(std::function<void()> lambdaFunc);

// リリースビルドでのみ実行される処理
void ProcessAtReleaseBuild(std::function<void()> lambdaFunc);

// デバッグログを出力する
void OutputDebugLog(const char* fmt, ...);

// ファイルを指定した場所にコピペする処理
void CopyFileToDestination(const WCHAR* srcPath, const WCHAR* destFolder, std::wstring* newPath = nullptr);

// 簡易的なfor文
void Loop(const uint32_t count, std::function<void()> func);

std::string WStrToStr(const std::wstring& wstr);