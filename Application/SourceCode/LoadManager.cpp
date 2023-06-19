#include "LoadManager.h"
#include "NewEngine.h"
#include "FbxLoader.h"
#include "AssimpLoader.h"

bool LoadManager::ModelLoad()
{
	//ModelManager::LoadFbxModel("boneTest", "BoneTest");
	//ModelManager::LoadObjModel("Sphere", "Sphere");
	//ModelManager::LoadObjModel("SkyDome", "SkyDome");
	//ModelManager::LoadObjModel("Ground", "Ground");
	//ModelManager::LoadObjModel("Enemy", "Enemy", true);
	//ModelManager::LoadObjModel("SphereCollider", "SphereCollider");
	//ModelManager::LoadFbxModel("AttackEnemy", "AttackEnemy");
	//ModelManager::LoadObjModel("player", "Player", true);
	//ModelManager::LoadObjModel("Plane", "Plane");
	ModelManager::LoadObjModel("Block1", "Block1");
	ModelManager::LoadFbxModel("Cube", "Cube");

	// �������I������̂������邽�߁A�K��true��Ԃ�
	return true;
}

bool LoadManager::TextureLoad()
{
	//TextureManager::CreateTexture(Color::white, "White");

	TextureManager::LoadTexture("pic.png", "pic");
	TextureManager::LoadTexture("BackGround.png", "BackGround");

	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 2, "Task");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Vignette");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "HighLumi");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "GaussainBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Bloom");

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
	}

	// �R�}���h���s
	TextureManager::ExcuteComandList();
}