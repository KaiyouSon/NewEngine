#pragma once

class Timer
{
private:
	int timer_;
	int limitTimer_;
	bool isTimeOut_;

public:
	Timer();
	Timer(const int maxTimer);

	void Reset();
	void Update(const bool isRoop, const int addTimer = 1);

public:	// セッター
	inline void SetLimitTimer(const int maxTimer) { this->limitTimer_ = maxTimer; }

public:	// ゲッター
	inline int GetTimer() { return timer_; }
	inline float GetTimeRate() { return (float)timer_ / (float)limitTimer_; }
	inline bool GetisTimeOut() { return isTimeOut_; }
};

