#pragma once
#include "Sound.h"
#include <string>
#include <map>
#include <xaudio2.h>

// サウンドを管理するクラス
class SoundManager
{
private:
	static IXAudio2MasteringVoice* sMasterVoice;
	static Microsoft::WRL::ComPtr<IXAudio2> sXAudio2;
	static std::map<std::string, std::unique_ptr<Sound>> sSoundMap;

public:
	// サウンドのロード
	static Sound* LoadSound(std::string filePath, std::string soundTag);

	// サウンドの取得
	static Sound* GetSound(std::string soundTag);

	// 再生
	static void Play(std::string soundTag, bool isRoop = false);

	// 停止
	static void Stop(std::string soundTag);

	// 再生中チェック
	static bool GetIsPlaying(std::string soundTag);

	// 音量設定
	static void SetVolume(std::string soundTag, float volume);

	// ピッチ設定
	static void SetPitch(std::string soundTag, float pitch);

	// マップ取得
	static std::map<std::string, std::unique_ptr<Sound>>* GetSoundMap();

public:
	static void Init();
	static void Destroy();
	static IXAudio2* GetXAudio2();
};

