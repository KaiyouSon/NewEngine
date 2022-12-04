#include "Sound.h"
#include <cassert>

void SoundManager::Initialize()
{
	HRESULT result;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

Sound SoundManager::LoadSoundWave(const char* filePath)
{
	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(filePath, std::ios_base::binary);
	// ファイルオープン失敗を検出
	assert(file.is_open());

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

	// returnするための音声データ
	Sound soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

void SoundManager::PlaySoundWave(const Sound& soundData)
{
	HRESULT result;

	// 波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = soundData.pBuffer;
	buffer.AudioBytes = soundData.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	// 波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buffer);
	result = pSourceVoice->Start();
}

void SoundManager::UnLoadSoundWave(Sound* soundData)
{
	// バッファのメモリ解放
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
