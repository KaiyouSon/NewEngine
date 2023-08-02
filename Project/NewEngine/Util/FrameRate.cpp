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
	this->mFrameRate = frameRate;
}

void FrameRate::Update()
{
	// ‚P/‚U‚O•b‚Ò‚Á‚½‚è‚ÌŠÔ
	const microseconds minTime(uint64_t(1000000.0f / mFrameRate));

	// ‚P/‚U‚O•b‚æ‚è‚í‚¸‚©‚É’Z‚¢ŠÔ
	const microseconds minCheckTime(uint64_t(1000000.0f / (mFrameRate + 5)));

	// Œ»İ‚ÌŠÔ‚ğæ“¾
	steady_clock::time_point nowTime = steady_clock::now();

	// ‘O‰ñ‚Ì‹L˜^‚©‚çŒo‰ßŠÔ‚ğæ“¾‚·‚é
	microseconds elapsedTime = duration_cast<microseconds>(nowTime - mReference);

	// ‚P/‚U‚O•bŒo‚Á‚Ä‚È‚¢ê‡
	if (elapsedTime < minCheckTime)
	{
		// ‚P/‚U‚O•bŒo‰ß‚·‚é‚Ü‚Å”÷¬‚ÈƒXƒŠ[ƒv‚ğŒJ‚è•Ô‚·
		while (steady_clock::now() - mReference < minTime)
		{
			sleep_for(microseconds(1));
		}
	}

	// Œ»İ‚ÌŠÔ‚ğ‹L˜^‚·‚é
	mReference = steady_clock::now();
}
