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
	// �P/ mFrameRate�b�҂�����̎���
	const microseconds minTime(uint64_t(1000000.0f / mFrameRate));

	// �P/ mFrameRate�b���킸���ɒZ������
	const microseconds minCheckTime(uint64_t(1000000.0f / (mFrameRate + 5)));

	// ���݂̎��Ԃ��擾
	steady_clock::time_point nowTime = steady_clock::now();

	// �O��̋L�^����o�ߎ��Ԃ��擾����
	microseconds elapsedTime = duration_cast<microseconds>(nowTime - mReference);

	// �P/mFrameRate�b�o���ĂȂ��ꍇ
	if (elapsedTime < minCheckTime)
	{
		// �P/mFrameRate�b�o�߂���܂Ŕ����ȃX���[�v���J��Ԃ�
		while (steady_clock::now() - mReference < minTime)
		{
			sleep_for(microseconds(1));
			nowTime = steady_clock::now();
		}
	}

	// ���݂�FPS
	mCurrentFPS = 1.0f / static_cast<float>(elapsedTime.count() / 1000000.0f);

	// ���݂̎��Ԃ��L�^����
	mReference = steady_clock::now();
}

float FrameRate::GetCurrentFPS()
{
	return mCurrentFPS;
}
