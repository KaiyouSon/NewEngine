#pragma once
#include "Vec3.h"
#include "Easing.h"
#include "NewEngineEnum.h"
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
	Vec3 InterPolation(const BezierType type = BezierType::Lerp);	// 陬憺俣

public:

	// 蛻ｶ蠕｡轤ｹ繧定ｿｽ蜉
	inline void AddPoint(const Vec3& pos) { mPoints.push_back(pos); }

	// 陬憺俣譎る俣繧偵そ繝・ヨ縺吶ｋ
	inline void SetEaseTime(const int32_t easeTime) { mEase.SetEaseTimer(easeTime); }

	// N荵励ｒ繧ｻ繝・ヨ縺吶ｋ
	inline void SetEasePowNum(const float pownum) { mEase.SetPowNum(pownum); }

	// 蛻ｶ蠕｡轤ｹ蜑企勁
	inline void ClearPoints() { mPoints.clear(); }

	// 邨ゆｺ・ヵ繝ｩ繧ｰ
	inline bool GetisEnd() { return mIsEnd; }

private:
	std::vector<Vec3> RecursiveLerp(const std::vector<Vec3>& points, const BezierType type);
};
