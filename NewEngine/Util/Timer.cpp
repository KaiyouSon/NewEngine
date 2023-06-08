#include "Timer.h"

Timer::Timer() : timer_(0), limitTimer_(0), isTimeOut_(false)
{
}

Timer::Timer(const int maxTimer) : timer_(0), limitTimer_(maxTimer), isTimeOut_(false)
{
}

void Timer::Reset()
{
	timer_ = 0;
	isTimeOut_ = false;
}

void Timer::Update(const bool isRoop, const int addTimer)
{
	if (isTimeOut_ == true)
	{
		if (timer_ >= limitTimer_)
		{
			Reset();
		}
	}
	else
	{
		timer_ += addTimer;

		if (timer_ >= limitTimer_)
		{
			isTimeOut_ = true;
		}
		else
		{
			isTimeOut_ = false;
		}
	}
}