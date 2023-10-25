#pragma once
#include <cstdint>
#include <wrl.h>
#include <xaudio2.h>

// チャンクヘッダ
struct ChunkHeader
{
    char id[4];     // チャンク識別子
    int32_t size;   // チャンクデータサイズ
};

// RIFFヘッダ
struct RiffHeader
{
    ChunkHeader chunk;  // "RIFF"
    char type[4];       // "WAVE"
};

// フォーマットチャンク
struct FormatChunk
{
    ChunkHeader chunk;   // "fmt "
    WAVEFORMATEX fmt;    // 波形形式情報
};

// 波形データ
struct WaveData
{
    // 波形形式情報
    WAVEFORMATEX wfex;
    // 波形データを格納するバッファ
    BYTE* pBuffer;
    // 波形データのサイズ
    uint32_t bufferSize;

    IXAudio2SourceVoice* pSourceVoice;
};