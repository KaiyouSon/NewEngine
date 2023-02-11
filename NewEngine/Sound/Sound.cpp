#include "Sound.h"
#include "SoundManager.h"
#include <cassert>
using namespace std;

Sound::Sound() : result(HRESULT()), soundData(SoundData()), volume(1)
{
}

Sound::Sound(const std::string& filePath) :
	result(HRESULT()), soundData(SoundData()), volume(1)
{
	string path = "Application/Sound/" + filePath;

	// �t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	// .wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(path, std::ios_base::binary);
	// �t�@�C���I�[�v�����s�����o
	if (file.is_open() == false)
	{
		assert(0 && "�T�E���h�t�@�C���̓ǂݍ��݂����s���܂���");
	}

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

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;
}

void Sound::Play(const bool& isRoop)
{
	// �g�`�t�H�[�}�b�g������SourceVoice�̐���
	result = SoundManager::GetXAudio2()->
		CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// �Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = soundData.pBuffer;
	buffer.AudioBytes = soundData.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (isRoop == true)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// �g�`�f�[�^�̍Đ�
	if (buffer.LoopCount >= 0)
	{
		result = soundData.pSourceVoice->SubmitSourceBuffer(&buffer);
		result = soundData.pSourceVoice->Start();
	}

	buffer.LoopCount--;
}

void Sound::Stop()
{
	if (soundData.pSourceVoice == nullptr)	return;
	soundData.pSourceVoice->Stop();
}

bool Sound::GetIsPlaying()
{
	if (soundData.pSourceVoice == nullptr) return false;

	XAUDIO2_VOICE_STATE xa2state{};
	soundData.pSourceVoice->GetState(&xa2state);

	return xa2state.BuffersQueued != 0;
}

void Sound::SetVolume(const float& volume)
{
	this->volume = volume;
	if (this->volume >= 1)
	{
		soundData.pSourceVoice->SetVolume(1);
	}
	else if (this->volume <= 0)
	{
		soundData.pSourceVoice->SetVolume(0);
	}
	else
	{
		soundData.pSourceVoice->SetVolume(volume);
	}
}

void Sound::UnLoad()
{
	// �o�b�t�@�̃��������
	delete[] soundData.pBuffer;

	soundData.pBuffer = 0;
	soundData.bufferSize = 0;
	soundData.wfex = {};
}
