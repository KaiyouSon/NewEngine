#pragma once
#include <cstdint>
#include <wrl.h>
#include <xaudio2.h>

// 繝√Ε繝ｳ繧ｯ繝倥ャ繝
struct ChunkHeader
{
	char id[4];		// 繝√Ε繝ｳ繧ｯ豈弱・ID
	int32_t size;	// 繝√Ε繝ｳ繧ｯ繧ｵ繧､繧ｺ
};

// RIFF繝倥ャ繝繝√Ε繝ｳ繧ｯ
struct RiffHeader
{
	ChunkHeader chunk;	// "RIFF"
	char type[4];		// "WAVE"
};

// FMT繝√Ε繝ｳ繧ｯ
struct FormatChunk
{
	ChunkHeader chunk;	// "fmt"
	WAVEFORMATEX fmt;	// 豕｢蠖｢繝輔か繝ｼ繝槭ャ繝・
};

// 髻ｳ螢ｰ繝・・繧ｿ
struct WaveData
{
	// 豕｢蠖｢繝輔か繝ｼ繝槭ャ繝・
	WAVEFORMATEX wfex;
	// 繝舌ャ繝輔ぃ縺ｮ蜈磯ｭ繧｢繝峨Ξ繧ｹ
	BYTE* pBuffer;
	// 繝舌ャ繝輔ぃ縺ｮ繧ｵ繧､繧ｺ
	uint32_t bufferSize;

	IXAudio2SourceVoice* pSourceVoice;
};
