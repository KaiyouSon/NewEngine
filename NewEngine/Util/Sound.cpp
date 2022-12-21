#include "Sound.h"
#include <cassert>
using namespace std;

IXAudio2MasteringVoice* Sound::masterVoice = nullptr;
Microsoft::WRL::ComPtr<IXAudio2> Sound::xAudio2 = nullptr;

void Sound::Init()
{
	HRESULT result;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

Sound::Sound() : result(HRESULT()), soundData(SoundData()), volume(1)
{
}

Sound::Sound(const std::string& filePath) :
	result(HRESULT()), soundData(SoundData()), volume(1)
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

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;
}

void Sound::Play(const bool& isRoop)
{
	// 波形フォーマットを元にSourceVoiceの生成
	//IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = soundData.pBuffer;
	buffer.AudioBytes = soundData.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (isRoop == true)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// 波形データの再生
	result = soundData.pSourceVoice->SubmitSourceBuffer(&buffer);
	result = soundData.pSourceVoice->Start();
}

void Sound::Stop()
{
	if (soundData.pSourceVoice == nullptr)	return;
	soundData.pSourceVoice->Stop();
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
	soundData.pSourceVoice->Stop();

	// バッファのメモリ解放
	delete[] soundData.pBuffer;

	soundData.pBuffer = 0;
	soundData.bufferSize = 0;
	soundData.wfex = {};
}
