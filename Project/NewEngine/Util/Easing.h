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
	void Reset();        // リセット
	void Update();       // 更新

public:	// セッター
	void SetEaseTimer(const int32_t limitTimer);
	void SetPowNum(const float powNum);
	void SetEaseType(const EaseType easeType);
	void SetisEnd(const bool isEnd);

public: // ゲッター
	bool GetisEnd();
	int32_t GetTimer();
	float GetTimeRate();
	uint32_t GetEaseTimer();

public:

	// 補間
	float Interpolation(const float startPos, const float endPos);
	Vec2 Interpolation(const Vec2 startPos, const Vec2 endPos);
	Vec3 Interpolation(const Vec3 startPos, const Vec3 endPos);

	// ラープ
	float Lerp(const float startPos, const float endPos);
	Vec2 Lerp(const Vec2 startPos, const Vec2 endPos);
	Vec3 Lerp(const Vec3 startPos, const Vec3 endPos);

	// イン
	float In(const float startPos, const float endPos);
	Vec2 In(const Vec2 startPos, const Vec2 endPos);
	Vec3 In(const Vec3 startPos, const Vec3 endPos);

	// アウト
	float Out(const float startPos, const float endPos);
	Vec2 Out(const Vec2 startPos, const Vec2 endPos);
	Vec3 Out(const Vec3 startPos, const Vec3 endPos);

	// インアウト
	float InOut(const float startPos, const float endPos);
	Vec2 InOut(const Vec2 startPos, const Vec2 endPos);
	Vec3 InOut(const Vec3 startPos, const Vec3 endPos);

	// インバック
	float InBack(const float startPos, const float endPos);
	Vec2 InBack(const Vec2 startPos, const Vec2 endPos);
	Vec3 InBack(const Vec3 startPos, const Vec3 endPos);
};

