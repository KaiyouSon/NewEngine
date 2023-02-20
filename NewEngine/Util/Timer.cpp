#include "Timer.h"

Timer::Timer() : timer(0), maxTimer(0), isTimeOut(false)
{
}

Timer::Timer(const int& maxTimer) : timer(0), maxTimer(maxTimer), isTimeOut(false)
{
}

void Timer::Reset()
{
	timer = 0;
	isTimeOut = false;
}

void Timer::AddTimer(const float& addTimer)
{
	timer++;
	if (timer >= maxTimer)
	{
		timer = maxTimer;
		isTimeOut = true;
	}
	else
	{
		isTimeOut = false;
	}
}

void Timer::SubTimer(const float& subTimer)
{
	timer++;
	if (timer <= maxTimer)
	{
		timer = maxTimer;
		isTimeOut = true;
	}
	else
	{
		isTimeOut = false;
	}
}