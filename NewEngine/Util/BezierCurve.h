#pragma once
#include <vector>
#include "Vec3.h"
#include "Easing.h"

enum BezierCurveType
{
	Lerp,
	EaseIn,
	EaseOut,
};

class BezierCurve
{
private:
	std::vector<Vec3> points;
	size_t startIndex;
	int timer;
	float timeRate;
	bool isEnd;

	Easing ease;

public:
	BezierCurve();
	BezierCurve(const int& easeTimer);
	BezierCurve(const int& easeTimer, const float& powNum);
	void Reset();
	void Update();
	Vec3 InterPolation(const BezierCurveType& type = BezierCurveType::Lerp);	// 補間

public:

	// 制御点を追加
	inline void AddPoint(const Vec3& pos) { points.push_back(pos); }

	// 補間時間をセットする
	inline void SetEaseTime(const int& easeTime) { ease.SetEaseTimer(easeTime); }

	// N乗をセットする
	inline void SetEasePowNum(const float& pownum) { ease.SetPowNum(pownum); }

	// 制御点削除
	inline void ClearPoints() { points.clear(); }

	// 終了フラグ
	inline bool GetisEnd() { return isEnd; }

private:
	std::vector<Vec3> RecursiveLerp(const std::vector<Vec3>& points, const BezierCurveType& type);
};