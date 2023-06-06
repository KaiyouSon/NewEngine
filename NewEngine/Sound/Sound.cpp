#include "Sound.h"
#include "SoundManager.h"
#include "Util.h"
#include <cassert>
using namespace std;

Sound::Sound() :
	result(HRESULT()), waveData(WaveData()), volume(1), pitch(1)
{
}

Sound::Sound(std::string filePath) :
	result(HRESULT()), waveData(WaveData()), volume(1), pitch(1)
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

	waveData.wfex = format.fmt;
	waveData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	waveData.bufferSize = data.size;
}

void Sound::Play(bool isRoop)
{
	// �g�`�t�H�[�}�b�g������SourceVoice�̐���
	result = SoundManager::GetXAudio2()->
		CreateSourceVoice(&waveData.pSourceVoice, &waveData.wfex);
	assert(SUCCEEDED(result));

	// �Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = waveData.pBuffer;
	buffer.AudioBytes = waveData.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (isRoop == true)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// �g�`�f�[�^�̍Đ�
	if (buffer.LoopCount >= 0)
	{
		result = waveData.pSourceVoice->SubmitSourceBuffer(&buffer);
		result = waveData.pSourceVoice->Start();
	}

	buffer.LoopCount--;
}

void Sound::Stop()
{
	// null�`�F�b�N
	if (waveData.pSourceVoice == nullptr) return;

	waveData.pSourceVoice->Stop();
}

bool Sound::GetIsPlaying()
{
	// null�`�F�b�N
	if (waveData.pSourceVoice == nullptr) return false;

	// ���
	XAUDIO2_VOICE_STATE xa2state{};
	waveData.pSourceVoice->GetState(&xa2state);

	return (xa2state.BuffersQueued != 0);
}

void Sound::SetVolume(float volume)
{
	// null�`�F�b�N
	if (waveData.pSourceVoice == nullptr) return;

	// clamp���ăZ�b�g����
	this->volume = Clamp(volume);
	waveData.pSourceVoice->SetVolume(this->volume);
}

void Sound::SetPitch(float pitch)
{
	// null�`�F�b�N
	if (waveData.pSourceVoice == nullptr) return;

	// clamp���ăZ�b�g����
	this->pitch = Clamp(pitch, 0, 2);
	waveData.pSourceVoice->SetFrequencyRatio(this->pitch);
}

void Sound::UnLoad()
{
	// �o�b�t�@�̃��������
	delete[] waveData.pBuffer;

	waveData.pBuffer = 0;
	waveData.bufferSize = 0;
	waveData.wfex = {};
}
