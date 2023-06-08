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
	Vec3 InterPolation(const BezierType type = BezierType::Lerp);	// ���

public:

	// ����_��ǉ�
	inline void AddPoint(const Vec3 pos) { points_.push_back(pos); }

	// ��Ԏ��Ԃ��Z�b�g����
	inline void SetEaseTime(const int easeTime) { ease_.SetEaseTimer(easeTime); }

	// N����Z�b�g����
	inline void SetEasePowNum(const float pownum) { ease_.SetPowNum(pownum); }

	// ����_�폜
	inline void ClearPoints() { points_.clear(); }

	// �I���t���O
	inline bool GetisEnd() { return isEnd_; }

private:
	std::vector<Vec3> RecursiveLerp(const std::vector<Vec3>& points, const BezierType type);
};