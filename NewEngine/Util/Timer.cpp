#include "Timer.h"

Timer::Timer() : timer_(0), limitTimer_(0), isTimeOut_(false)
{
}

Timer::Timer(const int32_t maxTimer) : timer_(0), limitTimer_(maxTimer), isTimeOut_(false)
{
}

void Timer::Reset()
{
	timer_ = 0;
	isTimeOut_ = false;
}

void Timer::Update(const bool isRoop, const int32_t addTimer)
{
	timer_ += addTimer;

	if (timer_ >= limitTimer_)
	{
		isTimeOut_ = true;
		Reset();

	}
	else
	{
		isTimeOut_ = false;
	}

	timeRate_ = (float)(timer_) / (float)limitTimer_;
}