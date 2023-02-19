#include "LoadManager.h"
#include "NewEngine.h"
#include "FbxLoader.h"

void LoadManager::ModelLoad()
{
	//ModelManager::LoadModel("Cube", "Cube", true);
	ModelManager::LoadObjModel("Sphere", "Sphere", true);
	ModelManager::LoadObjModel("SkyDome", "SkyDome");
	ModelManager::LoadObjModel("Ground", "Ground");
	ModelManager::LoadObjModel("Enemy", "Enemy", true);
	ModelManager::LoadObjModel("SphereCollider", "SphereCollider");
	ModelManager::LoadFbxModel("AttackEnemy", "AttackEnemy");
	ModelManager::LoadObjModel("player", "Player");
}

void LoadManager::TextureLoad()
{
	TextureManager::CreateTexture(Color::white, "White");
	TextureManager::LoadTexture("pic.png", "pic");
	TextureManager::CreateRenderTexture(GetWindowSize(), "PostEffect");

	//TextureManager::ExcuteComandList();
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
