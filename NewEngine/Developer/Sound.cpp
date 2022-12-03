#include "Sound.h"
#include <cassert>

void SoundManager::Initialize()
{
	HRESULT result;

	// XAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// �}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

Sound SoundManager::LoadSoundWave(const char* filePath)
{
	// �t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	// .wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(filePath, std::ios_base::binary);
	// �t�@�C���I�[�v�����s�����o
	assert(file.is_open());

	// RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// �t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)	assert(0);
	// �^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0) assert(0);

	// Format�`�����N�̓ǂݍ���
	FormatChunk format = {};
	file.read((char*)&format, sizeof(ChunkHeader));
	// �`�����N�w�b�_�[�̊m�F
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) assert(0);
	// �`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNK�`�����N�����������ꍇ
	if (strncmp(data.id, "JUNK ", 4) == 0)
	{
		// �ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		// �ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}
	// LIST�`�����N�����������ꍇ
	if (strncmp(data.id, "LIST ", 4) == 0)
	{
		// �ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		// �ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0) assert(0);

	// Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// wave�t�@�C�������
	file.close();

	// return���邽�߂̉����f�[�^
	Sound soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

void SoundManager::PlaySoundWave(const Sound& soundData)
{
	HRESULT result;

	// �g�`�t�H�[�}�b�g������SourceVoice�̐���
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// �Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = soundData.pBuffer;
	buffer.AudioBytes = soundData.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	// �g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buffer);
	result = pSourceVoice->Start();
}

void SoundManager::UnLoadSoundWave(Sound* soundData)
{
	// �o�b�t�@�̃��������
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

SoundManager* soundManager = SoundManager::GetInstance().get();

Sound LoadSoundWave(const char* filePath)
{
	return soundManager->LoadSoundWave(filePath);
}

void PlaySoundWave(const Sound& soundData)
{
	soundManager->PlaySoundWave(soundData);
}

void UnLoadSoundWave(Sound* soundData)
{
	soundManager->UnLoadSoundWave(soundData);
}
