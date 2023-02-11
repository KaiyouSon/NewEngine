#include "LoadManager.h"

void LoadManager::ModelLoad()
{
	//ModelManager::LoadModel("Cube", "Cube", true);
	//ModelManager::LoadModel("Sphere", "Sphere", true);
	//ModelManager::LoadModel("SkyDome", "SkyDome");
	//ModelManager::LoadModel("Ground", "Ground");
	//ModelManager::LoadModel("AttackEnemy", "AttackEnemy", true);
	//ModelManager::LoadModel("player", "Player", true);
}

void LoadManager::TextureLoad()
{
	//TextureManager::CreateTexture(Color::white, "White");
	TextureManager::LoadTexture("pic.png", "pic");
}

void LoadManager::SoundLoad()
{
}

void LoadManager::Load()
{
	TextureLoad();
	ModelLoad();
	SoundLoad();
}
