#pragma once
#include "Vec3.h"
#include <cstdint>

class Easing
{
private:
	uint32_t timer_;
	uint32_t limitTimer_;
	float timeRate_;
	float powNum_;
	bool isEnd_;

public:
	Easing();
	Easing(const uint32_t limitTimer);
	Easing(const uint32_t limitTimer, const float powNum);
	void Reset();		// ���Z�b�g
	void Update(const bool isRoop);		// ���

public: // �Z�b�^�[
	
	// ��Ԏ��Ԃ��Z�b�g����
	inline void SetEaseTimer(const uint32_t limitTimer) { limitTimer_ = limitTimer; }
	// N����Z�b�g����
	inline void SetPowNum(const float powNum) { powNum_ = powNum; }

public: // �Q�b�^�[

	// �C�[�W���O�I���
	inline bool GetisEnd() { return isEnd_; }
	// �^�C�}�[�擾
	inline uint32_t GetTimer() { return timer_; }
	// �^�C�����[�g���擾
	inline float GetTimeRate() { return timeRate_; }

public:

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
