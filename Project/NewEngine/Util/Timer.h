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

public:	// �Z�b�^�[
	inline void SetLimitTimer(const int32_t maxTimer) { mLimitTimer = maxTimer; }

public:	// �Q�b�^�[
	inline int32_t GetTimer() { return mTimer; }
	inline float GetTimeRate() { return mTimeRate; }
	inline bool GetisTimeOut() { return mIsTimeOut; }

public:
	/// <summary>
	/// bool GetisTimeOut()�Ɠ���
	/// </summary>
	bool operator==(const bool flag);
};

