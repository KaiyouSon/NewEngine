#include "FrameRate.h"
#include <thread>

using namespace std::chrono;
using namespace std::this_thread;

FrameRate::FrameRate() : mFrameRate(0)
{
}

void FrameRate::Init(const float frameRate)
{
	mReference = steady_clock::now();
	mFrameRate = frameRate;
}

void FrameRate::Update()
{
	// ・・ mFrameRate遘偵・縺｣縺溘ｊ縺ｮ譎る俣
	const microseconds minTime(uint64_t(1000000.0f / mFrameRate));

	// ・・ mFrameRate遘偵ｈ繧翫ｏ縺壹°縺ｫ遏ｭ縺・凾髢・
	const microseconds minCheckTime(uint64_t(1000000.0f / (mFrameRate + 5)));

	// 迴ｾ蝨ｨ縺ｮ譎る俣繧貞叙蠕・
	steady_clock::time_point nowTime = steady_clock::now();

	// 蜑榊屓縺ｮ險倬鹸縺九ｉ邨碁℃譎る俣繧貞叙蠕励☆繧・
	microseconds elapsedTime = duration_cast<microseconds>(nowTime - mReference);

	// ・・mFrameRate遘堤ｵ後▲縺ｦ縺ｪ縺・ｴ蜷・
	if (elapsedTime < minCheckTime)
	{
		// ・・mFrameRate遘堤ｵ碁℃縺吶ｋ縺ｾ縺ｧ蠕ｮ蟆上↑繧ｹ繝ｪ繝ｼ繝励ｒ郢ｰ繧願ｿ斐☆
		while (steady_clock::now() - mReference < minTime)
		{
			sleep_for(microseconds(1));
			nowTime = steady_clock::now();
		}
	}

	// 迴ｾ蝨ｨ縺ｮFPS
	mCurrentFPS = 1.0f / static_cast<float>(elapsedTime.count() / 1000000.0f);

	// 迴ｾ蝨ｨ縺ｮ譎る俣繧定ｨ倬鹸縺吶ｋ
	mReference = steady_clock::now();
}

float FrameRate::GetCurrentFPS()
{
	return mCurrentFPS;
}

