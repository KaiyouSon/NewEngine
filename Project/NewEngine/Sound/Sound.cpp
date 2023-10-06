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

	// 繝輔ぃ繧､繝ｫ蜈･蜉帙せ繝医Μ繝ｼ繝縺ｮ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ
	std::ifstream file;
	// .wav繝輔ぃ繧､繝ｫ繧偵ヰ繧､繝翫Μ繝｢繝ｼ繝峨〒髢九￥
	file.open(path, std::ios_base::binary);
	// 繝輔ぃ繧､繝ｫ繧ｪ繝ｼ繝励Φ螟ｱ謨励ｒ讀懷・
	if (file.is_open() == false)
	{
		assert(0 && "繧ｵ繧ｦ繝ｳ繝峨ヵ繧｡繧､繝ｫ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ縺悟､ｱ謨励＠縺ｾ縺励◆");
	}

	// RIFF繝倥ャ繝繝ｼ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// 繝輔ぃ繧､繝ｫ縺軍IFF縺九メ繧ｧ繝・け
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)	assert(0);
	// 繧ｿ繧､繝励′WAVE縺九メ繧ｧ繝・け
	if (strncmp(riff.type, "WAVE", 4) != 0) assert(0);

	// Format繝√Ε繝ｳ繧ｯ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
	FormatChunk format = {};
	file.read((char*)&format, sizeof(ChunkHeader));
	// 繝√Ε繝ｳ繧ｯ繝倥ャ繝繝ｼ縺ｮ遒ｺ隱・
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) assert(0);
	// 繝√Ε繝ｳ繧ｯ譛ｬ菴薙・隱ｭ縺ｿ霎ｼ縺ｿ
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Data繝√Ε繝ｳ繧ｯ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNK繝√Ε繝ｳ繧ｯ繧呈､懈渊縺励◆蝣ｴ蜷・
	if (strncmp(data.id, "JUNK ", 4) == 0)
	{
		// 隱ｭ縺ｿ蜿悶ｊ菴咲ｽｮ繧谷UNK繝√Ε繝ｳ繧ｯ縺ｮ邨ゅｏ繧翫∪縺ｧ騾ｲ繧√ｋ
		file.seekg(data.size, std::ios_base::cur);
		// 蜀崎ｪｭ縺ｿ霎ｼ縺ｿ
		file.read((char*)&data, sizeof(data));
	}
	// LIST繝√Ε繝ｳ繧ｯ繧呈､懈渊縺励◆蝣ｴ蜷・
	if (strncmp(data.id, "LIST ", 4) == 0)
	{
		// 隱ｭ縺ｿ蜿悶ｊ菴咲ｽｮ繧谷UNK繝√Ε繝ｳ繧ｯ縺ｮ邨ゅｏ繧翫∪縺ｧ騾ｲ繧√ｋ
		file.seekg(data.size, std::ios_base::cur);
		// 蜀崎ｪｭ縺ｿ霎ｼ縺ｿ
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0) assert(0);

	// Data繝√Ε繝ｳ繧ｯ縺ｮ繝・・繧ｿ驛ｨ・域ｳ｢蠖｢繝・・繧ｿ・峨・隱ｭ縺ｿ霎ｼ縺ｿ
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// wave繝輔ぃ繧､繝ｫ繧帝哩縺倥ｋ
	file.close();

	mWaveData.wfex = format.fmt;
	mWaveData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	mWaveData.bufferSize = data.size;
}

void Sound::Play(bool isRoop)
{
	// 豕｢蠖｢繝輔か繝ｼ繝槭ャ繝医ｒ蜈・↓SourceVoice縺ｮ逕滓・
	mResult = SoundManager::GetXAudio2()->
		CreateSourceVoice(&mWaveData.pSourceVoice, &mWaveData.wfex);
	assert(SUCCEEDED(mResult));

	// 蜀咲函縺吶ｋ豕｢蠖｢繝・・繧ｿ縺ｮ險ｭ螳・
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = mWaveData.pBuffer;
	buffer.AudioBytes = mWaveData.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (isRoop == true)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// 豕｢蠖｢繝・・繧ｿ縺ｮ蜀咲函
	if (buffer.LoopCount >= 0)
	{
		mResult = mWaveData.pSourceVoice->SubmitSourceBuffer(&buffer);
		mResult = mWaveData.pSourceVoice->Start();
	}

	buffer.LoopCount--;
}

void Sound::Stop()
{
	// null繝√ぉ繝・け
	if (mWaveData.pSourceVoice == nullptr) return;

	mWaveData.pSourceVoice->Stop();
}

bool Sound::GetIsPlaying()
{
	// null繝√ぉ繝・け
	if (mWaveData.pSourceVoice == nullptr) return false;

	// 迥ｶ諷・
	XAUDIO2_VOICE_STATE xa2state{};
	mWaveData.pSourceVoice->GetState(&xa2state);

	return (xa2state.BuffersQueued != 0);
}

void Sound::SetVolume(float volume)
{
	// null繝√ぉ繝・け
	if (mWaveData.pSourceVoice == nullptr) return;

	// clamp縺励※繧ｻ繝・ヨ縺吶ｋ
	mVolume = Clamp(volume);
	mWaveData.pSourceVoice->SetVolume(mVolume);
}

void Sound::SetPitch(float pitch)
{
	// null繝√ぉ繝・け
	if (mWaveData.pSourceVoice == nullptr) return;

	// clamp縺励※繧ｻ繝・ヨ縺吶ｋ
	mPitch = Clamp<float>(pitch, 0, 2);
	mWaveData.pSourceVoice->SetFrequencyRatio(mPitch);
}

void Sound::UnLoad()
{
	// 繝舌ャ繝輔ぃ縺ｮ繝｡繝｢繝ｪ隗｣謾ｾ
	delete[] mWaveData.pBuffer;

	mWaveData.pBuffer = 0;
	mWaveData.bufferSize = 0;
	mWaveData.wfex = {};
}

