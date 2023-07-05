#pragma once
#include <cstdint>

class Timer
{
private:
	int32_t timer_;
	int32_t limitTimer_;
	float timeRate_;
	bool isTimeOut_;

public:
	Timer();
	Timer(const int32_t maxTimer);

	void Reset();
	void Update(const bool isRoop, const int32_t addTimer = 1);

public:	// �Z�b�^�[
	inline void SetLimitTimer(const int32_t maxTimer) { limitTimer_ = maxTimer; }

public:	// �Q�b�^�[
	inline int32_t GetTimer() { return timer_; }
	inline float GetTimeRate() { return timeRate_; }
	inline bool GetisTimeOut() { return isTimeOut_; }
};

