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

public:	// セッター
	inline void SetLimitTimer(const int32_t maxTimer) { mLimitTimer = maxTimer; }

public:	// ゲッター
	inline int32_t GetTimer() { return mTimer; }
	inline float GetTimeRate() { return mTimeRate; }
	inline bool GetisTimeOut() { return mIsTimeOut; }

public:
	/// <summary>
	/// bool GetisTimeOut()と同じ
	/// </summary>
	bool operator==(const bool flag);
};

