#pragma once
#include <cstdint>
#include <wrl.h>
#include <xaudio2.h>

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
struct WaveData
{
	// 波形フォーマット
	WAVEFORMATEX wfex;
	// バッファの先頭アドレス
	BYTE* pBuffer;
	// バッファのサイズ
	uint32_t bufferSize;

	IXAudio2SourceVoice* pSourceVoice;
};