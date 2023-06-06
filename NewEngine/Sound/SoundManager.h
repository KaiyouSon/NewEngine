#pragma once
#include "Sound.h"
#include <string>
#include <map>
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

class SoundManager
{
private:
	static IXAudio2MasteringVoice* masterVoice;
	static Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
	static std::map<std::string, std::unique_ptr<Sound>> soundMap;

public:
	static Sound* GetSound(std::string soundTag);
	static Sound* LoadSound(std::string filePath, std::string soundTag);
	static void Play(std::string soundTag, bool isRoop = false);
	static void Stop(std::string soundTag);
	static bool GetIsPlaying(std::string soundTag);
	static void SetVolume(std::string soundTag, float volume);
	static void SetPitch( std::string soundTag,  float pitch);

	static void Init();
	static void Destroy();
	static inline IXAudio2* GetXAudio2() { return xAudio2.Get(); }
};
