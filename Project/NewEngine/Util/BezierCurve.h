#pragma once
#include "Vec3.h"
#include "Easing.h"
#include "Enum.h"
#include <vector>
#include <cstdint>

class BezierCurve
{
private:
	std::vector<Vec3> mPoints;
	uint32_t mStartIndex;
	int32_t mTimer;
	float mTimeRate;
	bool mIsEnd;

	Easing mEase;

public:
	BezierCurve();
	BezierCurve(const int32_t easeTimer);
	BezierCurve(const int32_t easeTimer, const float powNum);
	void Reset();
	void Update();
	Vec3 InterPolation(const BezierType type = BezierType::Lerp);	// 補間

public:

	// 制御点を追加
	inline void AddPoint(const Vec3& pos) { mPoints.push_back(pos); }

	// 補間時間をセットする
	inline void SetEaseTime(const int32_t easeTime) { mEase.SetEaseTimer(easeTime); }

	// N乗をセットする
	inline void SetEasePowNum(const float pownum) { mEase.SetPowNum(pownum); }

	// 制御点削除
	inline void ClearPoints() { mPoints.clear(); }

	// 終了フラグ
	inline bool GetisEnd() { return mIsEnd; }

private:
	std::vector<Vec3> RecursiveLerp(const std::vector<Vec3>& points, const BezierType type);
};