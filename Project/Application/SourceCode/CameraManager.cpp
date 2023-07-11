#include "CameraManager.h"
#include "DefaultCamera.h"

CameraManager::CameraManager() :
	defCame_(std::make_unique<Camera>())
{
}

void CameraManager::Init()
{
	currentCamera_ = std::make_unique<DefaultCamera>();
	currentCamera_->Init(player_);
}

void CameraManager::Update()
{
	currentCamera_->Update();
}

void CameraManager::SetPlayer(Player* player)
{
	player_ = player;
}