#include "CameraManager.h"
#include "DefaultCamera.h"
#include "TargetCamera.h"


CameraManager::CameraManager()
{
}

void CameraManager::Init()
{
	currentCamera_ = std::make_unique<DefaultCamera>();
	currentCamera_->Init(player_);

	cameraType_ = CameraType::Default;
}

void CameraManager::Update()
{
	if (Pad::GetButtonDown(PadCode::RightStick))
	{
		if (cameraType_ == CameraType::Default)
		{
			ChangeCamera(CameraType::Target);
		}
		else if (cameraType_ == CameraType::Target)
		{
			ChangeCamera(CameraType::Default);
		}
	}

	currentCamera_->Update();
}

void CameraManager::ChangeCamera(const CameraType cameraType)
{
	cameraType_ = cameraType;

	std::unique_ptr<ICamera> nextCamera;
	switch (cameraType)
	{
	case CameraType::Default:
		nextCamera = std::make_unique<DefaultCamera>();
		break;

	case CameraType::Target:
		nextCamera = std::make_unique<TargetCamera>();
		break;

	default:
		break;
	}

	nextCamera->Init(player_);
	currentCamera_ = std::move(nextCamera);
}

CameraManager::CameraType CameraManager::GetCameraType()
{
	return cameraType_;
}

void CameraManager::SetPlayer(Player* player)
{
	player_ = player;
}