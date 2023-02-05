#include "FrameRate.h"
#include <thread>

using namespace std::chrono;
using namespace std::this_thread;

FrameRate::FrameRate() : frameRate(0)
{
}

void FrameRate::Init(const float& frameRate)
{
	reference = steady_clock::now();
	this->frameRate = frameRate;
}

void FrameRate::Update()
{
	// �P/�U�O�b�҂�����̎���
	const microseconds minTime(uint64_t(1000000.0f / frameRate));

	// �P/�U�O�b���킸���ɒZ������
	const microseconds minCheckTime(uint64_t(1000000.0f / (frameRate + 5)));

	// ���݂̎��Ԃ��擾
	steady_clock::time_point nowTime = steady_clock::now();

	// �O��̋L�^����o�ߎ��Ԃ��擾����
	microseconds elapsedTime = duration_cast<microseconds>(nowTime - reference);

	// �P/�U�O�b�o���ĂȂ��ꍇ
	if (elapsedTime < minCheckTime)
	{
		// �P/�U�O�b�o�߂���܂Ŕ����ȃX���[�v���J��Ԃ�
		while (steady_clock::now() - reference < minTime)
		{
			sleep_for(microseconds(1));
		}
	}

	// ���݂̎��Ԃ��L�^����
	reference = steady_clock::now();


}
