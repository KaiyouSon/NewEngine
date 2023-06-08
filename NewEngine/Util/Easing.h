#pragma once
#include "Vec3.h"
#include <cstdint>

class Easing
{
private:
	uint32_t timer_;
	uint32_t limitTimer_;
	float timeRate_;
	float powNum_;
	bool isEnd_;

public:
	Easing();
	Easing(const uint32_t limitTimer);
	Easing(const uint32_t limitTimer, const float powNum);
	void Reset();		// リセット
	void Update(const bool isRoop);		// 補間

public: // セッター
	
	// 補間時間をセットする
	inline void SetEaseTimer(const uint32_t limitTimer) { limitTimer_ = limitTimer; }
	// N乗をセットする
	inline void SetPowNum(const float powNum) { powNum_ = powNum; }

public: // ゲッター

	// イージング終わり
	inline bool GetisEnd() { return isEnd_; }
	// タイマー取得
	inline uint32_t GetTimer() { return timer_; }
	// タイムレートを取得
	inline float GetTimeRate() { return timeRate_; }

public:

	// ラープ
	float Lerp(const float startPos, const float endPos);
	Vec2 Lerp(const Vec2 startPos, const Vec2 endPos);
	Vec3 Lerp(const Vec3 startPos, const Vec3 endPos);

	// イーズイン
	float In(const float startPos, const float endPos);
	Vec2 In(const Vec2 startPos, const Vec2 endPos);
	Vec3 In(const Vec3 startPos, const Vec3 endPos);

	// イーズアウト
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
