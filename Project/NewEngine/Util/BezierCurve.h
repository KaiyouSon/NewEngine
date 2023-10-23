#pragma once
#include "Vec3.h"
#include "Easing.h"
#include "NewEngineEnum.h"
#include <vector>
#include <cstdint>

// ベジエ曲線のクラス
class BezierCurve
{
private:
	std::vector<Vec3> mPoints;
	uint32_t mStartIndex;
	int32_t mTimer;
	float mTimeRate;
	bool mIsEnd;

	Easing mEase;

private:
	// 再帰関数
	std::vector<Vec3> RecursiveLerp(const std::vector<Vec3>& points, const BezierType type);

public:
	BezierCurve();
	BezierCurve(const int32_t easeTimer);
	BezierCurve(const int32_t easeTimer, const float powNum);
	void Reset();
	void Update();

	// 補間
	Vec3 InterPolation(const BezierType type = BezierType::Lerp);

public: // 制御点関連
	void AddPoint(const Vec3& pos);
	void ClearPoints();

public: // セッター
	void SetEaseTime(const int32_t easeTime);
	void SetEasePowNum(const float pownum);

public: // ゲッター
	bool GetisEnd();
};