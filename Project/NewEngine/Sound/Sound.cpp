#include "Sound.h"
#include "SoundManager.h"
#include "Util.h"
#include <cassert>
using namespace std;

Sound::Sound() :
	mResult(HRESULT()), mWaveData(WaveData()), mVolume(1), mPitch(1)
{
}

Sound::Sound(std::string filePath) :
	mResult(HRESULT()), mWaveData(WaveData()), mVolume(1), mPitch(1)
{
	string path = "Application/Sound/" + filePath;

	// 音声ファイルの読み込み
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(path, std::ios_base::binary);
	// ファイルが開けない場合はエラー
	if (file.is_open() == false)
	{
		assert(0 && "音声ファイルの読み込みに失敗しました");
	}

	// RIFFチャンクの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFチャンクで始まらない場合はエラー
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) assert(0);
	// ファイルがWAVE形式でない場合はエラー
	if (strncmp(riff.type, "WAVE", 4) != 0) assert(0);

	// Formatチャンクの読み込み
	FormatChunk format = {};
	file.read((char*)&format, sizeof(ChunkHeader));
	// チャンクが"fmt "で始まらない場合はエラー
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) assert(0);
	// チャンクサイズが正当な範囲内か確認
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクの場合、スキップして読み込む
	if (strncmp(data.id, "JUNK ", 4) == 0)
	{
		// スキップして読み込む
		file.seekg(data.size, std::ios_base::cur);
		// 次のチャンクを読み込む
		file.read((char*)&data, sizeof(data));
	}
	// LISTチャンクの場合、スキップして読み込む
	if (strncmp(data.id, "LIST ", 4) == 0)
	{
		// スキップして読み込む
		file.seekg(data.size, std::ios_base::cur);
		// 次のチャンクを読み込む
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0) assert(0);

	// Dataチャンクのデータを読み込む
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// waveデータをファイルを閉じる
	file.close();

	mWaveData.wfex = format.fmt;
	mWaveData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	mWaveData.bufferSize = data.size;
}

void Sound::Play(bool isLoop)
{
	// 音声データからSourceVoiceを作成
	mResult = SoundManager::GetXAudio2()->
		CreateSourceVoice(&mWaveData.pSourceVoice, &mWaveData.wfex);
	assert(SUCCEEDED(mResult));

	// 音声データをバッファにセット
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = mWaveData.pBuffer;
	buffer.AudioBytes = mWaveData.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (isLoop == true)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// バッファを設定
	if (buffer.LoopCount >= 0)
	{
		mResult = mWaveData.pSourceVoice->SubmitSourceBuffer(&buffer);
		mResult = mWaveData.pSourceVoice->Start();
	}

	buffer.LoopCount--;
}

void Sound::Stop()
{
	// nullポインタの確認
	if (mWaveData.pSourceVoice == nullptr) return;

	mWaveData.pSourceVoice->Stop();
}

bool Sound::GetIsPlaying()
{
	if (mWaveData.pSourceVoice == nullptr) return false;

	// 状態を取得
	XAUDIO2_VOICE_STATE xa2state{};
	mWaveData.pSourceVoice->GetState(&xa2state);

	return (xa2state.BuffersQueued != 0);
}

void Sound::SetVolume(float volume)
{
	// nullポインタの確認
	if (mWaveData.pSourceVoice == nullptr) return;

	// 音量を設定（クランプ処理を適用）
	mVolume = Clamp(volume);
	mWaveData.pSourceVoice->SetVolume(mVolume);
}

void Sound::SetPitch(float pitch)
{
	// nullポインタの確認
	if (mWaveData.pSourceVoice == nullptr) return;

	// ピッチを設定（クランプ処理を適用）
	mPitch = Clamp<float>(pitch, 0, 2);
	mWaveData.pSourceVoice->SetFrequencyRatio(mPitch);
}

void Sound::UnLoad()
{
	// 音声データの解放
	delete[] mWaveData.pBuffer;

	mWaveData.pBuffer = 0;
	mWaveData.bufferSize = 0;
	mWaveData.wfex = {};
}

