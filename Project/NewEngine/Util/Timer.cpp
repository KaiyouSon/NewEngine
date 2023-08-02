#include "Timer.h"

Timer::Timer() : mTimer(0), mLimitTimer(0), mIsTimeOut(false)
{
}

Timer::Timer(const int32_t maxTimer) : mTimer(0), mLimitTimer(maxTimer), mIsTimeOut(false)
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
	return mIsTimeOut;
}
