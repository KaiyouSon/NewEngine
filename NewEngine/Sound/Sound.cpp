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

	waveData.wfex = format.fmt;
	waveData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	waveData.bufferSize = data.size;
}

void Sound::Play(bool isRoop)
{
	// 波形フォーマットを元にSourceVoiceの生成
	result = SoundManager::GetXAudio2()->
		CreateSourceVoice(&waveData.pSourceVoice, &waveData.wfex);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = waveData.pBuffer;
	buffer.AudioBytes = waveData.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (isRoop == true)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// 波形データの再生
	if (buffer.LoopCount >= 0)
	{
		result = waveData.pSourceVoice->SubmitSourceBuffer(&buffer);
		result = waveData.pSourceVoice->Start();
	}

	buffer.LoopCount--;
}

void Sound::Stop()
{
	// nullチェック
	if (waveData.pSourceVoice == nullptr) return;

	waveData.pSourceVoice->Stop();
}

bool Sound::GetIsPlaying()
{
	// nullチェック
	if (waveData.pSourceVoice == nullptr) return false;

	// 状態
	XAUDIO2_VOICE_STATE xa2state{};
	waveData.pSourceVoice->GetState(&xa2state);

	return (xa2state.BuffersQueued != 0);
}

void Sound::SetVolume(float volume)
{
	// nullチェック
	if (waveData.pSourceVoice == nullptr) return;

	// clampしてセットする
	this->volume = Clamp(volume);
	waveData.pSourceVoice->SetVolume(this->volume);
}

void Sound::SetPitch(float pitch)
{
	// nullチェック
	if (waveData.pSourceVoice == nullptr) return;

	// clampしてセットする
	this->pitch = Clamp(pitch, 0, 2);
	waveData.pSourceVoice->SetFrequencyRatio(this->pitch);
}

void Sound::UnLoad()
{
	// バッファのメモリ解放
	delete[] waveData.pBuffer;

	waveData.pBuffer = 0;
	waveData.bufferSize = 0;
	waveData.wfex = {};
}
