#pragma once
#include <vector>
#include "Vec3.h"
#include "Easing.h"
#include "Enum.h"

class BezierCurve
{
private:
	std::vector<Vec3> points_;
	size_t startIndex_;
	int timer_;
	float timeRate_;
	bool isEnd_;

	Easing ease_;

public:
	BezierCurve();
	BezierCurve(const int easeTimer);
	BezierCurve(const int easeTimer, const float powNum);
	void Reset();
	void Update();
	Vec3 InterPolation(const BezierType type = BezierType::Lerp);	// 補間

public:

	// 制御点を追加
	inline void AddPoint(const Vec3 pos) { points_.push_back(pos); }

	// 補間時間をセットする
	inline void SetEaseTime(const int easeTime) { ease_.SetEaseTimer(easeTime); }

	// N乗をセットする
	inline void SetEasePowNum(const float pownum) { ease_.SetPowNum(pownum); }

	// 制御点削除
	inline void ClearPoints() { points_.clear(); }

	// 終了フラグ
	inline bool GetisEnd() { return isEnd_; }

private:
	std::vector<Vec3> RecursiveLerp(const std::vector<Vec3>& points, const BezierType type);
};