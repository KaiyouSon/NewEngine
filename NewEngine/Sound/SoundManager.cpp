#include "SoundManager.h"

IXAudio2MasteringVoice* SoundManager::masterVoice_ = nullptr;
Microsoft::WRL::ComPtr<IXAudio2> SoundManager::xAudio2_ = nullptr;
std::map<std::string, std::unique_ptr<Sound>> SoundManager::soundMap_;

Sound* SoundManager::GetSound(std::string soundTag)
{
	return soundMap_[soundTag].get();
}

Sound* SoundManager::LoadSound(std::string filePath, std::string soundTag)
{
	std::unique_ptr<Sound> sound = std::make_unique<Sound>(filePath);
	soundMap_.insert(std::make_pair(soundTag, std::move(sound)));

	return soundMap_[soundTag].get();
}

void SoundManager::Play(std::string soundTag, bool isRoop)
{
	soundMap_[soundTag]->Play(isRoop);
}

void SoundManager::Stop(std::string soundTag)
{
	soundMap_[soundTag]->Stop();
}

bool SoundManager::GetIsPlaying(std::string soundTag)
{
	return soundMap_[soundTag]->GetIsPlaying();
}

void SoundManager::SetVolume(std::string soundTag, float volume)
{
	soundMap_[soundTag]->SetVolume(volume);
}

void SoundManager::SetPitch(std::string soundTag, float pitch)
{
	soundMap_[soundTag]->SetPitch(pitch);
}

void SoundManager::Init()
{
	HRESULT result;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// マスターボイスを生成
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
}

void SoundManager::Destroy()
{
	xAudio2_.Reset();

	for (auto& sound : soundMap_)
	{
		soundMap_[sound.first]->UnLoad();
	}
}
