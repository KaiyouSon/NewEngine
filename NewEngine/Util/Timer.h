#pragma once

class Timer
{
private:
	int timer;
	int maxTimer;
	bool isTimeOut;

public:
	Timer();
	Timer(const int& maxTimer);

	void Reset();
	void AddTimer(const float& addTimer = 1);
	void SubTimer(const float& subTimer = 1);
	inline void SetMaxTimer(const int& maxTimer) { this->maxTimer = maxTimer; }
	inline bool GetisTimeOut() { return isTimeOut; }
	inline float GetTimeRate() { return (float)timer / (float)maxTimer; }
};

