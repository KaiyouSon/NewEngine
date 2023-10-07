#pragma once
#include "Vec3.h"
#include "NewEngineEnum.h"
#include <cstdint>

class Easing
{
private:
	int32_t mTimer;
	int32_t mLimitTimer;
	float mTimeRate;
	float mPowNum;
	bool mIsEnd;
	EaseType mEaseType;

public:
	Easing();
	Easing(const int32_t limitTimer);
	Easing(const int32_t limitTimer, const float powNum);
	void Reset();		// 繝ｪ繧ｻ繝・ヨ
	void Update();		// 陬憺俣

public: // 繧ｻ繝・ち繝ｼ

	// 陬憺俣譎る俣繧偵そ繝・ヨ縺吶ｋ
	inline void SetEaseTimer(const int32_t limitTimer) { mLimitTimer = limitTimer; }
	// N荵励ｒ繧ｻ繝・ヨ縺吶ｋ
	inline void SetPowNum(const float powNum) { mPowNum = powNum; }
	inline void SetEaseType(const EaseType easeType) { mEaseType = easeType; }
	inline void SetisEnd(const bool isEnd) { mIsEnd = isEnd; }

public: // 繧ｲ繝・ち繝ｼ

	// 繧､繝ｼ繧ｸ繝ｳ繧ｰ邨ゅｏ繧・
	bool GetisEnd();
	// 繧ｿ繧､繝槭・蜿門ｾ・
	int32_t GetTimer();
	// 繧ｿ繧､繝繝ｬ繝ｼ繝医ｒ蜿門ｾ・
	float GetTimeRate();
	// 陬憺俣繧ｿ繧､繝繧貞叙蠕・
	uint32_t GetEaseTimer();

public:

	float Interpolation(const float startPos, const float endPos);
	Vec2 Interpolation(const Vec2 startPos, const Vec2 endPos);
	Vec3 Interpolation(const Vec3 startPos, const Vec3 endPos);

	// 繝ｩ繝ｼ繝・
	float Lerp(const float startPos, const float endPos);
	Vec2 Lerp(const Vec2 startPos, const Vec2 endPos);
	Vec3 Lerp(const Vec3 startPos, const Vec3 endPos);

	// 繧､繝ｼ繧ｺ繧､繝ｳ
	float In(const float startPos, const float endPos);
	Vec2 In(const Vec2 startPos, const Vec2 endPos);
	Vec3 In(const Vec3 startPos, const Vec3 endPos);

	// 繧､繝ｼ繧ｺ繧｢繧ｦ繝・
	float Out(const float startPos, const float endPos);
	Vec2 Out(const Vec2 startPos, const Vec2 endPos);
	Vec3 Out(const Vec3 startPos, const Vec3 endPos);

	// 繧､繝ｳ繧｢繧ｦ繝・
	float InOut(const float startPos, const float endPos);
	Vec2 InOut(const Vec2 startPos, const Vec2 endPos);
	Vec3 InOut(const Vec3 startPos, const Vec3 endPos);

	// 繧､繝ｳ繝舌ャ繧ｯ
	float InBack(const float startPos, const float endPos);
	Vec2 InBack(const Vec2 startPos, const Vec2 endPos);
	Vec3 InBack(const Vec3 startPos, const Vec3 endPos);
};

