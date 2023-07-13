#include "CameraManager.h"
#include "DefaultCamera.h"
#include "TargetCamera.h"

CameraManager::CameraManager()
{
	cameraType_ = CameraType::Default;
}

void CameraManager::Init()
{
	currentCamera_ = std::make_unique<DefaultCamera>();
	currentCamera_->Init(player_);
}

void CameraManager::Update()
{
	currentCamera_->SetLockonPos(boss_->GetPos());
	currentCamera_->Update();

	if (Camera::current.rot.y >= Radian(360))
	{
		Camera::current.rot.y -= Radian(360);
	}
	else if (Camera::current.rot.y <= -Radian(360))
	{
		Camera::current.rot.y += Radian(360);
	}

	bool isRightStickDown = Pad::GetButtonDown(PadCode::RightStick);
	if (cameraType_ == CameraType::Default)
	{
		if (isRightStickDown)
		{
			Vec3 v1 = player_->GetPos() - Camera::current.pos;
			Vec3 v2 = currentCamera_->GetLockonPos() - player_->GetPos();

			float dot = Vec3::Dot(v1.Norm(), v2.Norm());
			if (dot >= cosf(Radian(80)) &&
				v2.Length() <= 100)
			{
				ChangeCamera(CameraType::Target);
			}
			else
			{
				if (Pad::GetStick(PadCode::LeftStick, 300) == 0)
				{
					currentCamera_->SetisEase(true);
				}
			}
		}
	}
	else if (cameraType_ == CameraType::Target)
	{
		if (isRightStickDown)
		{
			ChangeCamera(CameraType::Default);
		}

		float dis = Vec3(boss_->GetPos() - player_->GetPos()).Length();
		if (dis >= 100)
		{
			ChangeCamera(CameraManager::Default);
		}
	}
}

void CameraManager::ChangeCamera(const CameraType cameraType)
{
	cameraType_ = cameraType;

	std::unique_ptr<ICamera> nextCamera;
	switch (cameraType_)
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

void CameraManager::SetBoss(Boss* boss)
{
	boss_ = boss;
}
