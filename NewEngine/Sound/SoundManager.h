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
	static std::map<std::string, Sound> soundMap;

public:
	static Sound GetSound(const std::string& soundTag);
	static Sound LoadSound(const std::string& filePath, const std::string soundTag);
	static void Play(const std::string soundTag, const bool& isRoop = false);
	static void Stop(const std::string soundTag);
	static bool GetIsPlaying(const std::string soundTag);
	static void SetVolume(const std::string soundTag, const float& volume);

	static void Init();
	static void Destroy();
	static inline IXAudio2* GetXAudio2() { return xAudio2.Get(); }
};
