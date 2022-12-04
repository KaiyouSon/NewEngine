#include "DeveloperManager.h"
#include "InputManager.h"
#include "Sound.h"
#include "Util.h"

void DeveloperManager::Initialize()
{
	inputManager->Initialize();
	soundManager->Initialize();
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