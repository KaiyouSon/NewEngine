#pragma once
#include "Vec3.h"
#include "Enum.h"
#include <cstdint>

class Easing
{
private:
	int32_t timer_;
	int32_t limitTimer_;
	float timeRate_;
	float powNum_;
	bool isEnd_;
	EaseType easeType_;

public:
	Easing();
	Easing(const int32_t limitTimer);
	Easing(const int32_t limitTimer, const float powNum);
	void Reset();		// ���Z�b�g
	void Update();		// ���

public: // �Z�b�^�[

	// ��Ԏ��Ԃ��Z�b�g����
	inline void SetEaseTimer(const int32_t limitTimer) { limitTimer_ = limitTimer; }
	// N����Z�b�g����
	inline void SetPowNum(const float powNum) { powNum_ = powNum; }
	inline void SetEaseType(const EaseType easeType) { easeType_ = easeType; }
	inline void SetisEnd(const bool isEnd) { isEnd_ = isEnd; }

public: // �Q�b�^�[

	// �C�[�W���O�I���
	inline bool GetisEnd() { return isEnd_; }
	// �^�C�}�[�擾
	inline int32_t GetTimer() { return timer_; }
	// �^�C�����[�g���擾
	inline float GetTimeRate() { return timeRate_; }

public:

	float Interpolation(const float startPos, const float endPos);
	Vec2 Interpolation(const Vec2 startPos, const Vec2 endPos);
	Vec3 Interpolation(const Vec3 startPos, const Vec3 endPos);

	// ���[�v
	float Lerp(const float startPos, const float endPos);
	Vec2 Lerp(const Vec2 startPos, const Vec2 endPos);
	Vec3 Lerp(const Vec3 startPos, const Vec3 endPos);

	// �C�[�Y�C��
	float In(const float startPos, const float endPos);
	Vec2 In(const Vec2 startPos, const Vec2 endPos);
	Vec3 In(const Vec3 startPos, const Vec3 endPos);

	// �C�[�Y�A�E�g
	float Out(const float startPos, const float endPos);
	Vec2 Out(const Vec2 startPos, const Vec2 endPos);
	Vec3 Out(const Vec3 startPos, const Vec3 endPos);

	// �C���A�E�g
	float InOut(const float startPos, const float endPos);
	Vec2 InOut(const Vec2 startPos, const Vec2 endPos);
	Vec3 InOut(const Vec3 startPos, const Vec3 endPos);

	// �C���o�b�N
	float InBack(const float startPos, const float endPos);
	Vec2 InBack(const Vec2 startPos, const Vec2 endPos);
	Vec3 InBack(const Vec3 startPos, const Vec3 endPos);
};
