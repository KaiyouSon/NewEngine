#pragma once
#include <fstream>
#include "WaveSound.h"

class Sound
{
private:
	HRESULT result_;
	WaveData waveData_;
	float volume_;
	float pitch_;

public:
	Sound();
	Sound(std::string filePath);
	void Play(bool isRoop = false);
	void Stop();
	bool GetIsPlaying();
	void SetVolume(float volume);
	void SetPitch(float pitch);
	void UnLoad();
};
