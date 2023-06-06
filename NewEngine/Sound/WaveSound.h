#pragma once
#include <cstdint>
#include <wrl.h>
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

// �`�����N�w�b�_
struct ChunkHeader
{
	char id[4];		// �`�����N����ID
	int32_t size;	// �`�����N�T�C�Y
};

// RIFF�w�b�_�`�����N
struct RiffHeader
{
	ChunkHeader chunk;	// "RIFF"
	char type[4];		// "WAVE"
};

// FMT�`�����N
struct FormatChunk
{
	ChunkHeader chunk;	// "fmt"
	WAVEFORMATEX fmt;	// �g�`�t�H�[�}�b�g
};

// �����f�[�^
struct WaveData
{
	// �g�`�t�H�[�}�b�g
	WAVEFORMATEX wfex;
	// �o�b�t�@�̐擪�A�h���X
	BYTE* pBuffer;
	// �o�b�t�@�̃T�C�Y
	unsigned int bufferSize;

	IXAudio2SourceVoice* pSourceVoice;
};