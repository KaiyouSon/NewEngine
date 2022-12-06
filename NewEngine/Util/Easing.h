#pragma once
#include "Vec3.h"

class Easing
{
private:
	int timer;
	int easeTimer;
	float timeRate;
	float powNum;
	bool isEnd;

public:
	Easing();
	Easing(const int& easeTimer);
	Easing(const int& easeTimer, const float& powNum);
	void Reset();		// ���Z�b�g
	void Update();		// ���

	// ��Ԏ��Ԃ��Z�b�g����
	inline void SetEaseTimer(const int& easeTimer) { this->easeTimer = easeTimer; }
	// N����Z�b�g����
	inline void SetPowNum(const float& powNum) { this->powNum = powNum; }
	// �C�[�W���O�I���
	inline bool GetisEnd() { return isEnd; }

	// ���[�v
	float Lerp(const float& startPos, const float& endPos);
	Vec2 Lerp(const Vec2& startPos, const Vec2& endPos);
	Vec3 Lerp(const Vec3& startPos, const Vec3& endPos);

	// �C�[�Y�C��
	float In(const float& startPos, const float& endPos);
	Vec2 In(const Vec2& startPos, const Vec2& endPos);
	Vec3 In(const Vec3& startPos, const Vec3& endPos);

	// �C�[�Y�A�E�g
	float Out(const float& startPos, const float& endPos);
	Vec2 Out(const Vec2& startPos, const Vec2& endPos);
	Vec3 Out(const Vec3& startPos, const Vec3& endPos);

	// �C���A�E�g
	float InOut(const float& startPos, const float& endPos);
	Vec2 InOut(const Vec2& startPos, const Vec2& endPos);
	Vec3 InOut(const Vec3& startPos, const Vec3& endPos);

	// �C���o�b�N
	float InBack(const float& startPos, const float& endPos);
	Vec2 InBack(const Vec2& startPos, const Vec2& endPos);
	Vec3 InBack(const Vec3& startPos, const Vec3& endPos);
};
