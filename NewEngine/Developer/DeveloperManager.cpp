#include "DeveloperManager.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "ViewProjection.h"
#include "Sound.h"
#include "Scene.h"

void DeveloperManager::Initialize()
{
	inputManager->Initialize();
	soundManager->Initialize();
	view->Initialize();
	Random::Initialize();
}
void DeveloperManager::Update()
{
	inputManager->Update();
	sceneManager->Update();
}

void DeveloperManager::Draw()
{
	sceneManager->Draw();
}

DeveloperManager* developerManager = DeveloperManager::GetInstance().get();