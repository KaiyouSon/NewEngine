#include "LoadManager.h"
#include "NewEngine.h"
#include "FbxLoader.h"

void LoadManager::ModelLoad()
{
	ModelManager::LoadFbxModel("boneTest", "BoneTest");
	ModelManager::LoadObjModel("Sphere", "Sphere");
	ModelManager::LoadObjModel("SkyDome", "SkyDome");
	ModelManager::LoadObjModel("Ground", "Ground");
	ModelManager::LoadObjModel("Enemy", "Enemy", true);
	ModelManager::LoadObjModel("SphereCollider", "SphereCollider");
	ModelManager::LoadFbxModel("AttackEnemy", "AttackEnemy");
	ModelManager::LoadObjModel("player", "Player", true);
	ModelManager::LoadObjModel("Plane", "Plane");
}

void LoadManager::TextureLoad()
{
	TextureManager::CreateTexture(Color::white, "White");
	TextureManager::LoadTexture("pic.png", "pic");
	TextureManager::LoadTexture("number.png", "NumberSheet");

	TextureManager::ExcuteComandList();
}

void LoadManager::SoundLoad()
{
	SoundManager::LoadSound("Tutorial_BGM.wav", "BGM");
}

void LoadManager::Load()
{
	TextureLoad();
	ModelLoad();
	SoundLoad();
}
