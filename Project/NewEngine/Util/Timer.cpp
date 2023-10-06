#include "Timer.h"

Timer::Timer() :
	mTimer(0), mLimitTimer(0), mTimeRate(0), mIsTimeOut(false)
{
}

Timer::Timer(const int32_t maxTimer) :
	mTimer(0), mLimitTimer(maxTimer), mTimeRate(0), mIsTimeOut(false)
{
}

void Timer::Reset()
{
	mTimer = 0;
	mIsTimeOut = false;
}

void Timer::Update(const int32_t addTimer)
{
	mTimer += addTimer;

	if (mTimer >= mLimitTimer)
	{
		mTimer = mLimitTimer;
		mIsTimeOut = true;
	}
	else
	{
		mIsTimeOut = false;
	}

	mTimeRate = (float)(mTimer) / (float)mLimitTimer;
}

bool Timer::operator==(const bool flag)
{
	return mIsTimeOut == flag;
}

// 繧ｻ繝・ち繝ｼ
void Timer::SetTimer(const int32_t timer)
{
	mTimer = timer;
}
void Timer::SetLimitTimer(const int32_t maxTimer)
{
	mLimitTimer = maxTimer;
}

// 繧ｲ繝・ち繝ｼ
int32_t Timer::GetTimer()
{
	return mTimer;
}
float Timer::GetTimeRate()
{
	return mTimeRate;
}
bool Timer::GetisTimeOut()
{
	return mIsTimeOut;
}
