#pragma once
#include <cstdint>

class Timer
{
private:
	int32_t mTimer;
	int32_t mLimitTimer;
	float mTimeRate;
	bool mIsTimeOut;

public:
	Timer();
	Timer(const int32_t maxTimer);

	void Reset();
	void Update(const int32_t addTimer = 1);

public:	// 繧ｻ繝・ち繝ｼ
	void SetLimitTimer(const int32_t maxTimer);
	void SetTimer(const int32_t timer);

public:	// 繧ｲ繝・ち繝ｼ
	int32_t GetTimer();
	float GetTimeRate();
	bool GetisTimeOut();

public:
	/// <summary>
	/// bool GetisTimeOut()縺ｨ蜷後§
	/// </summary>
	bool operator==(const bool flag);
};
