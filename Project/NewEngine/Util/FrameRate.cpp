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
	// 目標フレームレートに対する最小経過時間（マイクロ秒単位）
	const microseconds minTime(uint64_t(1000000.0f / mFrameRate));

	// 目標フレームレートに対する最小確認時間（マイクロ秒単位）
	const microseconds minCheckTime(uint64_t(1000000.0f / (mFrameRate + 5)));

	// 現在時刻を取得
	steady_clock::time_point nowTime = steady_clock::now();

	// 経過時間を計算
	microseconds elapsedTime = duration_cast<microseconds>(nowTime - mReference);

	// 目標フレームレートに達していない場合
	if (elapsedTime < minCheckTime)
	{
		// 目標フレームレートまで待機し、時間経過後に続行
		while (steady_clock::now() - mReference < minTime)
		{
			sleep_for(microseconds(1));
			nowTime = steady_clock::now();
		}
	}

	// 現在のFPS（フレームレート）
	mCurrentFPS = 1.0f / duration<float>(elapsedTime).count();

	// 現在時刻を更新
	mReference = steady_clock::now();
}

float FrameRate::GetCurrentFPS()
{
	return mCurrentFPS;
}

