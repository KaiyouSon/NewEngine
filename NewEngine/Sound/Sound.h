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
struct SoundData
{
	// 波形フォーマット
	WAVEFORMATEX wfex;
	// バッファの先頭アドレス
	BYTE* pBuffer;
	// バッファのサイズ
	unsigned int bufferSize;

	IXAudio2SourceVoice* pSourceVoice;
};

class Sound
{
private:
	HRESULT result;
	SoundData soundData;
	float volume;

public:
	Sound();
	Sound(const std::string& filePath);
	void Play(const bool& isRoop = false);
	void Stop();
	bool GetIsPlaying();
	void SetVolume(const float& volume);
	void UnLoad();
};
