#include "SoundManager.h"
#pragma comment(lib,"xaudio2.lib")

IXAudio2MasteringVoice* SoundManager::sMasterVoice = nullptr;
Microsoft::WRL::ComPtr<IXAudio2> SoundManager::sXAudio2 = nullptr;
std::map<std::string, std::unique_ptr<Sound>> SoundManager::sSoundMap;

Sound* SoundManager::GetSound(std::string soundTag)
{
	return sSoundMap[soundTag].get();
}

Sound* SoundManager::LoadSound(std::string filePath, std::string soundTag)
{
	std::unique_ptr<Sound> sound = std::make_unique<Sound>(filePath);
	sSoundMap.insert(std::make_pair(soundTag, std::move(sound)));

	return sSoundMap[soundTag].get();
}

void SoundManager::Play(std::string soundTag, bool isRoop)
{
	sSoundMap[soundTag]->Play(isRoop);
}

void SoundManager::Stop(std::string soundTag)
{
	sSoundMap[soundTag]->Stop();
}

bool SoundManager::GetIsPlaying(std::string soundTag)
{
	return sSoundMap[soundTag]->GetIsPlaying();
}

void SoundManager::SetVolume(std::string soundTag, float volume)
{
	sSoundMap[soundTag]->SetVolume(volume);
}

void SoundManager::SetPitch(std::string soundTag, float pitch)
{
	sSoundMap[soundTag]->SetPitch(pitch);
}

std::map<std::string, std::unique_ptr<Sound>>* SoundManager::GetSoundMap()
{
	return &sSoundMap;
}

void SoundManager::Init()
{
	HRESULT result;

	// XAudio繧ｨ繝ｳ繧ｸ繝ｳ縺ｮ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧堤函謌・
	result = XAudio2Create(&sXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// 繝槭せ繧ｿ繝ｼ繝懊う繧ｹ繧堤函謌・
	result = sXAudio2->CreateMasteringVoice(&sMasterVoice);
}

void SoundManager::Destroy()
{
	sXAudio2.Reset();

	for (auto& sound : sSoundMap)
	{
		sSoundMap[sound.first]->UnLoad();
	}
}

