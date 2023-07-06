#include "LoadManager.h"
#include "NewEngine.h"
#include "AssimpLoader.h"

bool LoadManager::ModelLoad()
{
	// �������I������̂������邽�߁A�K��true��Ԃ�
	return true;
}

bool LoadManager::TextureLoad()
{
	TextureManager::CreateTexture(Color::white, "White");
	TextureManager::CreateDepthTexture(Vec2(1920, 1080));

	// �������I������̂������邽�߁A�K��true��Ԃ�
	return true;
}

bool LoadManager::SoundLoad()
{
	// �������I������̂������邽�߁A�K��true��Ԃ�
	return true;
}

LoadManager::LoadManager() : isLoaded(false)
{
}

void LoadManager::Load()
{
	// �񓯊�
	std::future<bool> textureFtr = std::async(std::launch::async, [this] { return TextureLoad(); });
	std::future<bool> modelFtr = std::async(std::launch::async, [this] { return ModelLoad(); });
	std::future<bool> soundFtr = std::async(std::launch::async, [this] { return SoundLoad(); });

	// ���[�h����
	if (textureFtr.get() == true &&	// �e�N�X�`���[
		modelFtr.get() == true &&	// ���f��
		soundFtr.get() == true)		// �T�E���h
	{
		isLoaded = true;
		// �R�}���h���s
		TextureManager::ExcuteComandList();
	}

}