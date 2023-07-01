#include "LoadManager.h"
#include "NewEngine.h"
#include "AssimpLoader.h"

bool LoadManager::ModelLoad()
{
	ModelManager::LoadObjModel("SkyDome", "SkyDome");
	ModelManager::LoadObjModel("Block1", "Block1");
	ModelManager::LoadFbxModel("boneTest", "BoneTest");
	ModelManager::LoadObjModel("Cube", "Cube");

	ModelManager::LoadObjModel("HumanoidBody/Body", "Body");
	ModelManager::LoadObjModel("HumanoidBody/Head", "Head");
	ModelManager::LoadObjModel("HumanoidBody/Limbs", "Limbs");

	// �������I������̂������邽�߁A�K��true��Ԃ�
	return true;
}

bool LoadManager::TextureLoad()
{
	TextureManager::CreateTexture(Color::white, "White");

	TextureManager::LoadTexture("pic.png", "pic");
	TextureManager::LoadTexture("BackGround.png", "BackGround");
	TextureManager::LoadTexture("Gauge.png", "Gauge");

	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 2, "Task");

	// ���P�x���o
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "HighLumi");

	// �w�i
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "BackGround");

	// �h�b�h�t�B���^�[
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "DotFilter");

	// �r�l�b�g
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Vignette");

	// �u���[��
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "BGaussainBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Bloom");

	// �K�E�V�A���u���[
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "GaussianBlur");

	// �O���A
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "GLineBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Glare1");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Glare2");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Glare3");

	// ��ʊE�[�x
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "DOF");

	// ���W�A���u���[
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "RadialBlur");


	TextureManager::CreateDepthTexture(Vec2(1920, 1080));

	// �������I������̂������邽�߁A�K��true��Ԃ�
	return true;
}

bool LoadManager::SoundLoad()
{
	//SoundManager::LoadSound("GameBGM.wav", "GameBGM");

	// �������I������̂������邽�߁A�K��true��Ԃ�
	return true;
}

LoadManager::LoadManager() : isLoaded(false)
{
}

void LoadManager::Load()
{
	// ���[�h�O
	TextureManager::CreateTexture(Color::white, "White");

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