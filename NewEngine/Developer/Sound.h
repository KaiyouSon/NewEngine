#pragma once
#include "Singleton.h"
#include <wrl.h>
#include <fstream>
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

// チャンクヘッダ
struct ChunkHeader
{
	char id[4];		// チャンク毎のID
	int32_t size;	// チャンクサイズ
};

// RIFFヘッダチャンク
struct RiffHeader
{
	ChunkHeader chunk;	// "RIFF"
	char type[4];		// "WAVE"
};

// FMTチャンク
struct FormatChunk
{
	ChunkHeader chunk;	// "fmt"
	WAVEFORMATEX fmt;	// 波形フォーマット
};

// 音声データ
struct Sound
{
	// 波形フォーマット
	WAVEFORMATEX wfex;
	// バッファの先頭アドレス
	BYTE* pBuffer;
	// バッファのサイズ
	unsigned int bufferSize;
};

template<typename T> class Singleton;

class SoundManager : public Singleton<SoundManager>
{
private:
	friend Singleton<SoundManager>;
	IXAudio2MasteringVoice* masterVoice;
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;

public:
	void Initialize();

	Sound LoadSoundWave(const char* filePath);
	void PlaySoundWave(const Sound& soundData);
	void UnLoadSoundWave(Sound* soundData);
};

extern SoundManager* soundManager;

Sound LoadSoundWave(const char* filePath);
void PlaySoundWave(const Sound& soundData);
void UnLoadSoundWave(Sound* soundData);
