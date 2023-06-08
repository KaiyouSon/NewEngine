#include "Sound.h"
#include "SoundManager.h"
#include "Util.h"
#include <cassert>
using namespace std;

Sound::Sound() :
	result_(HRESULT()), waveData_(WaveData()), volume_(1), pitch_(1)
{
}

Sound::Sound(std::string filePath) :
	result_(HRESULT()), waveData_(WaveData()), volume_(1), pitch_(1)
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

	waveData_.wfex = format.fmt;
	waveData_.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	waveData_.bufferSize = data.size;
}

void Sound::Play(bool isRoop)
{
	// �g�`�t�H�[�}�b�g������SourceVoice�̐���
	result_ = SoundManager::GetXAudio2()->
		CreateSourceVoice(&waveData_.pSourceVoice, &waveData_.wfex);
	assert(SUCCEEDED(result_));

	// �Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = waveData_.pBuffer;
	buffer.AudioBytes = waveData_.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (isRoop == true)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// �g�`�f�[�^�̍Đ�
	if (buffer.LoopCount >= 0)
	{
		result_ = waveData_.pSourceVoice->SubmitSourceBuffer(&buffer);
		result_ = waveData_.pSourceVoice->Start();
	}

	buffer.LoopCount--;
}

void Sound::Stop()
{
	// null�`�F�b�N
	if (waveData_.pSourceVoice == nullptr) return;

	waveData_.pSourceVoice->Stop();
}

bool Sound::GetIsPlaying()
{
	// null�`�F�b�N
	if (waveData_.pSourceVoice == nullptr) return false;

	// ���
	XAUDIO2_VOICE_STATE xa2state{};
	waveData_.pSourceVoice->GetState(&xa2state);

	return (xa2state.BuffersQueued != 0);
}

void Sound::SetVolume(float volume)
{
	// null�`�F�b�N
	if (waveData_.pSourceVoice == nullptr) return;

	// clamp���ăZ�b�g����
	volume_ = Clamp(volume);
	waveData_.pSourceVoice->SetVolume(volume_);
}

void Sound::SetPitch(float pitch)
{
	// null�`�F�b�N
	if (waveData_.pSourceVoice == nullptr) return;

	// clamp���ăZ�b�g����
	pitch_ = Clamp<float>(pitch, 0, 2);
	waveData_.pSourceVoice->SetFrequencyRatio(pitch_);
}

void Sound::UnLoad()
{
	// �o�b�t�@�̃��������
	delete[] waveData_.pBuffer;

	waveData_.pBuffer = 0;
	waveData_.bufferSize = 0;
	waveData_.wfex = {};
}
