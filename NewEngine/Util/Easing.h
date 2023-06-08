#pragma once
#include "Vec3.h"

class Easing
{
private:
	int timer_;
	int limitTimer_;
	float timeRate_;
	float powNum_;
	bool isEnd_;

public:
	Easing();
	Easing(const int& limitTimer);
	Easing(const int& limitTimer, const float& powNum);
	void Reset();		// リセット
	void Update(const bool isRoop);		// 補間

	// 補間時間をセットする
	inline void SetEaseTimer(const int& limitTimer) { this->limitTimer_ = limitTimer; }
	// N乗をセットする
	inline void SetPowNum(const float& powNum) { this->powNum_ = powNum; }
	// イージング終わり
	inline bool GetisEnd() { return isEnd_; }
	// タイマー取得
	inline int GetTimer() { return timer_; }
	// タイムレートを取得
	inline float GetTimeRate() { return timeRate_; }

	// ラープ
	float Lerp(const float& startPos, const float& endPos);
	Vec2 Lerp(const Vec2& startPos, const Vec2& endPos);
	Vec3 Lerp(const Vec3& startPos, const Vec3& endPos);

	// イーズイン
	float In(const float& startPos, const float& endPos);
	Vec2 In(const Vec2& startPos, const Vec2& endPos);
	Vec3 In(const Vec3& startPos, const Vec3& endPos);

	// イーズアウト
	float Out(const float& startPos, const float& endPos);
	Vec2 Out(const Vec2& startPos, const Vec2& endPos);
	Vec3 Out(const Vec3& startPos, const Vec3& endPos);

	// インアウト
	float InOut(const float& startPos, const float& endPos);
	Vec2 InOut(const Vec2& startPos, const Vec2& endPos);
	Vec3 InOut(const Vec3& startPos, const Vec3& endPos);

	// インバック
	float InBack(const float& startPos, const float& endPos);
	Vec2 InBack(const Vec2& startPos, const Vec2& endPos);
	Vec3 InBack(const Vec3& startPos, const Vec3& endPos);
};
