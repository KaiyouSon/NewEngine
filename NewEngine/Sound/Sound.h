#pragma once
#include <fstream>
#include "WaveSound.h"

class Sound
{
private:
	HRESULT result;
	WaveData waveData;
	float volume;
	float pitch;

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
