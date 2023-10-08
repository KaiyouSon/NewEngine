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
#include "Rect.h"
#include "NewEngineDefine.h"
#include "NewEngineEnum.h"
#include "NewEngineSetting.h"
#include <functional>

static const int maxBoneIndices = 4;

// 豈碑ｼ・＠縺ｦ螟ｧ縺阪＞譁ｹ繧定ｿ斐☆
template<typename T>
T Max(const T a, const T b)
{
	return a >= b ? a : b;
}

// 豈碑ｼ・＠縺ｦ蟆上＆縺・婿繧定ｿ斐☆
template<typename T>
T Min(const T a, const T b)
{
	return a <= b ? a : b;
}

// 隨ｦ蜿ｷ繧定ｿ斐☆・・-1, 0, 1 ・・
uint32_t Sign(const float a);
Vec2 Sign(const Vec2 a);
Vec3 Sign(const Vec3 a);

// 蛟､繧樽in縺ｨMax縺ｮ髢薙↓蛻ｶ髯舌☆繧矩未謨ｰ
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

// 蛟､縺悟庶譚溘☆繧矩未謨ｰ
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

// 蛟､繧定ｶ・∴縺溘ｉ謌ｻ縺・
template<typename T>
T Restore(const T value, const T limit, const T origin = 0)
{
	if (value >= limit)
	{
		return origin + value - limit;
	}
	return value;
}

// 譯∵焚繧貞叙蠕・
uint32_t GetDight(const uint32_t value);

// 迴ｾ蝨ｨ譎る俣繧定ｿ斐☆髢｢謨ｰ
unsigned long GetNowTime(const TimeUnit timeUnit = TimeUnit::MilliSecond);

// 繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓吶ｒ繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ蠎ｧ讓吶↓螟画鋤縺吶ｋ
Vec2 WorldToScreen(const Vec3 worldPos);

// 荳芽ｧ貞ｽ｢縺ｮ豕慕ｷ壹ｒ豎ゅａ繧・
Vec3 GetTriangleNormal(const Vec3 p0, const Vec3 p1, const Vec3 p2);

// Debug繝薙Ν繝峨・縺ｿ螳溯｡後☆繧・
void ProcessAtDebugBulid(std::function<void()> lambdaFunc);

// Release繝薙Ν繝芽ｺｫ縺ｮ螳溯｡後☆繧・
void ProcessAtReleaseBulid(std::function<void()> lambdaFunc);

// 蜃ｺ蜉帙え繧｣繝ｳ繝峨え縺ｫ蜃ｺ蜉帙☆繧・
void OutputDebugLog(const char* fmt...);
