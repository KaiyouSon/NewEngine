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

	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(path, std::ios_base::binary);
	// ファイルオープン失敗を検出
	if (file.is_open() == false)
	{
		assert(0 && "サウンドファイルの読み込みが失敗しました");
	}

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)	assert(0);
	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) assert(0);

	// Formatチャンクの読み込み
	FormatChunk format = {};
	file.read((char*)&format, sizeof(ChunkHeader));
	// チャンクヘッダーの確認
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) assert(0);
	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクを検査した場合
	if (strncmp(data.id, "JUNK ", 4) == 0)
	{
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}
	// LISTチャンクを検査した場合
	if (strncmp(data.id, "LIST ", 4) == 0)
	{
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0) assert(0);

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// waveファイルを閉じる
	file.close();

	waveData_.wfex = format.fmt;
	waveData_.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	waveData_.bufferSize = data.size;
}

void Sound::Play(bool isRoop)
{
	// 波形フォーマットを元にSourceVoiceの生成
	result_ = SoundManager::GetXAudio2()->
		CreateSourceVoice(&waveData_.pSourceVoice, &waveData_.wfex);
	assert(SUCCEEDED(result_));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = waveData_.pBuffer;
	buffer.AudioBytes = waveData_.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (isRoop == true)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// 波形データの再生
	if (buffer.LoopCount >= 0)
	{
		result_ = waveData_.pSourceVoice->SubmitSourceBuffer(&buffer);
		result_ = waveData_.pSourceVoice->Start();
	}

	buffer.LoopCount--;
}

void Sound::Stop()
{
	// nullチェック
	if (waveData_.pSourceVoice == nullptr) return;

	waveData_.pSourceVoice->Stop();
}

bool Sound::GetIsPlaying()
{
	// nullチェック
	if (waveData_.pSourceVoice == nullptr) return false;

	// 状態
	XAUDIO2_VOICE_STATE xa2state{};
	waveData_.pSourceVoice->GetState(&xa2state);

	return (xa2state.BuffersQueued != 0);
}

void Sound::SetVolume(float volume)
{
	// nullチェック
	if (waveData_.pSourceVoice == nullptr) return;

	// clampしてセットする
	volume_ = Clamp(volume);
	waveData_.pSourceVoice->SetVolume(volume_);
}

void Sound::SetPitch(float pitch)
{
	// nullチェック
	if (waveData_.pSourceVoice == nullptr) return;

	// clampしてセットする
	pitch_ = Clamp<float>(pitch, 0, 2);
	waveData_.pSourceVoice->SetFrequencyRatio(pitch_);
}

void Sound::UnLoad()
{
	// バッファのメモリ解放
	delete[] waveData_.pBuffer;

	waveData_.pBuffer = 0;
	waveData_.bufferSize = 0;
	waveData_.wfex = {};
}
