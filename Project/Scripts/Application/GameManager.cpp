#include "GameManager.h"

GameManager::GameManager()
{
	tag = "GameManager";
}

void GameManager::Init()
{
}

void GameManager::Update()
{
	if (Key::GetKeyDown(DIK_1))
	{
		static std::string sceneName = "Game";
		SceneManager::ChangeScene(sceneName);
	}
}

std::shared_ptr<Script> GameManager::CreateInstence()
{
	return std::make_shared<GameManager>();
}
