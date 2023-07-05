#include "SoundManager.h"
#pragma comment(lib,"xaudio2.lib")

IXAudio2MasteringVoice* SoundManager::sMasterVoice_ = nullptr;
Microsoft::WRL::ComPtr<IXAudio2> SoundManager::sXAudio2_ = nullptr;
std::map<std::string, std::unique_ptr<Sound>> SoundManager::sSoundMap_;

Sound* SoundManager::GetSound(std::string soundTag)
{
	return sSoundMap_[soundTag].get();
}

Sound* SoundManager::LoadSound(std::string filePath, std::string soundTag)
{
	std::unique_ptr<Sound> sound = std::make_unique<Sound>(filePath);
	sSoundMap_.insert(std::make_pair(soundTag, std::move(sound)));

	return sSoundMap_[soundTag].get();
}

void SoundManager::Play(std::string soundTag, bool isRoop)
{
	sSoundMap_[soundTag]->Play(isRoop);
}

void SoundManager::Stop(std::string soundTag)
{
	sSoundMap_[soundTag]->Stop();
}

bool SoundManager::GetIsPlaying(std::string soundTag)
{
	return sSoundMap_[soundTag]->GetIsPlaying();
}

void SoundManager::SetVolume(std::string soundTag, float volume)
{
	sSoundMap_[soundTag]->SetVolume(volume);
}

void SoundManager::SetPitch(std::string soundTag, float pitch)
{
	sSoundMap_[soundTag]->SetPitch(pitch);
}

void SoundManager::Init()
{
	HRESULT result;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&sXAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// マスターボイスを生成
	result = sXAudio2_->CreateMasteringVoice(&sMasterVoice_);
}

void SoundManager::Destroy()
{
	sXAudio2_.Reset();

	for (auto& sound : sSoundMap_)
	{
		sSoundMap_[sound.first]->UnLoad();
	}
}
