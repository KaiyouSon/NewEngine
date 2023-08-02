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
	Vec3 InterPolation(const BezierType type = BezierType::Lerp);	// ���

public:

	// ����_��ǉ�
	inline void AddPoint(const Vec3& pos) { mPoints.push_back(pos); }

	// ��Ԏ��Ԃ��Z�b�g����
	inline void SetEaseTime(const int32_t easeTime) { mEase.SetEaseTimer(easeTime); }

	// N����Z�b�g����
	inline void SetEasePowNum(const float pownum) { mEase.SetPowNum(pownum); }

	// ����_�폜
	inline void ClearPoints() { mPoints.clear(); }

	// �I���t���O
	inline bool GetisEnd() { return mIsEnd; }

private:
	std::vector<Vec3> RecursiveLerp(const std::vector<Vec3>& points, const BezierType type);
};