#include "DeveloperManager.h"
#include "InputManager.h"
#include "ViewProjection.h"
#include "Sound.h"

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
}

void DeveloperManager::Draw()
{
}

DeveloperManager* developerManager = DeveloperManager::GetInstance().get();