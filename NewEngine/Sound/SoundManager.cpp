#include "SoundManager.h"

IXAudio2MasteringVoice* SoundManager::masterVoice = nullptr;
Microsoft::WRL::ComPtr<IXAudio2> SoundManager::xAudio2 = nullptr;
std::map<std::string, Sound> SoundManager::soundMap;

Sound SoundManager::GetSound(const std::string& soundTag)
{
    return soundMap[soundTag];
}

Sound SoundManager::LoadSound(const std::string& filePath, const std::string soundTag)
{
	Sound sound = Sound(filePath);
	soundMap.insert(std::make_pair(soundTag, sound));

	return sound;
}

void SoundManager::Play(const std::string soundTag, const bool& isRoop)
{
	soundMap[soundTag].Play(isRoop);
}

void SoundManager::Stop(const std::string soundTag)
{
	soundMap[soundTag].Stop();
}

bool SoundManager::GetIsPlaying(const std::string soundTag)
{
	return soundMap[soundTag].GetIsPlaying();
}

void SoundManager::SetVolume(const std::string soundTag, const float& volume)
{
	soundMap[soundTag].SetVolume(volume);
}

void SoundManager::Init()
{
	HRESULT result;

	// XAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// �}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

void SoundManager::Destroy()
{
	xAudio2.Reset();

	for (const auto sound : soundMap)
	{
		soundMap[sound.first].UnLoad();
	}
}
