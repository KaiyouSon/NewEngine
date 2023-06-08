#pragma once
#include <cstdint>

class Timer
{
private:
	uint32_t timer_;
	uint32_t limitTimer_;
	bool isTimeOut_;

public:
	Timer();
	Timer(const uint32_t maxTimer);

	void Reset();
	void Update(const bool isRoop, const uint32_t addTimer = 1);

public:	// セッター
	inline void SetLimitTimer(const uint32_t maxTimer) { this->limitTimer_ = maxTimer; }

public:	// ゲッター
	inline uint32_t GetTimer() { return timer_; }
	inline float GetTimeRate() { return (float)(timer_ / limitTimer_); }
	inline bool GetisTimeOut() { return isTimeOut_; }
};

