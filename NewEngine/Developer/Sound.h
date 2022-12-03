#pragma once
#include "Singleton.h"
#include <wrl.h>
#include <fstream>
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
struct Sound
{
	// �g�`�t�H�[�}�b�g
	WAVEFORMATEX wfex;
	// �o�b�t�@�̐擪�A�h���X
	BYTE* pBuffer;
	// �o�b�t�@�̃T�C�Y
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
