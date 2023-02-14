#include "LoadManager.h"
#include "NewEngine.h"
#include "FbxLoader.h"

void LoadManager::ModelLoad()
{
	ModelManager::LoadModel("Cube", "Cube", true);
	ModelManager::LoadModel("Sphere", "Sphere", true);
	ModelManager::LoadModel("SkyDome", "SkyDome");
	ModelManager::LoadModel("Ground", "Ground");
	ModelManager::LoadModel("AttackEnemy", "AttackEnemy", true);
	ModelManager::LoadModel("player", "Player", true);
	ModelManager::LoadModel("SphereCollider", "SphereCollider");

	FbxLoader::GetInstance()->Load("cube");
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
