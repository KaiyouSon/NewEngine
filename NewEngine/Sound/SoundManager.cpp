#include "SoundManager.h"

IXAudio2MasteringVoice* SoundManager::masterVoice = nullptr;
Microsoft::WRL::ComPtr<IXAudio2> SoundManager::xAudio2 = nullptr;
std::map<std::string, std::unique_ptr<Sound>> SoundManager::soundMap;

Sound* SoundManager::GetSound(std::string soundTag)
{
	return soundMap[soundTag].get();
}

Sound* SoundManager::LoadSound(std::string filePath, std::string soundTag)
{
	std::unique_ptr<Sound> sound = std::make_unique<Sound>(filePath);
	soundMap.insert(std::make_pair(soundTag, std::move(sound)));

	return soundMap[soundTag].get();
}

void SoundManager::Play(std::string soundTag, bool isRoop)
{
	soundMap[soundTag]->Play(isRoop);
}

void SoundManager::Stop(std::string soundTag)
{
	soundMap[soundTag]->Stop();
}

bool SoundManager::GetIsPlaying(std::string soundTag)
{
	return soundMap[soundTag]->GetIsPlaying();
}

void SoundManager::SetVolume(std::string soundTag, float volume)
{
	soundMap[soundTag]->SetVolume(volume);
}

void SoundManager::SetPitch(std::string soundTag, float pitch)
{
	soundMap[soundTag]->SetPitch(pitch);
}

void SoundManager::Init()
{
	HRESULT result;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

void SoundManager::Destroy()
{
	xAudio2.Reset();

	for (auto& sound : soundMap)
	{
		soundMap[sound.first]->UnLoad();
	}
}
