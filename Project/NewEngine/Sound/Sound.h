#pragma once
#include <fstream>
#include "WaveSound.h"

// サウンドのクラス
class Sound
{
private:
	HRESULT mResult;
	WaveData mWaveData;
	float mVolume;
	float mPitch;

public:
	Sound();
	Sound(std::string filePath);
	void Play(const bool isRoop = false);
	void Stop();
	bool GetIsPlaying();
	void SetVolume(const float volume);
	void SetPitch(const float pitch);
	void UnLoad();
};