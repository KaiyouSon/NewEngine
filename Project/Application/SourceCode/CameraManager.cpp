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
	currentCamera_->Init(mPlayer);
}

void CameraManager::Update()
{
	currentCamera_->SetLockonPos(mBoss->GetPos());
	currentCamera_->Update();

	if (Camera::current.rot.y >= Radian(360))
	{
		Camera::current.rot.y -= Radian(360);
	}
	else if (Camera::current.rot.y <= -Radian(360))
	{
		Camera::current.rot.y += Radian(360);
	}

	// ƒJƒƒ‰Ø‚è‘Ö‚¦‚éˆ—
	bool isRightStickDown = Pad::GetButtonDown(PadCode::RightStick);
	if (cameraType_ == CameraType::Default)
	{
		if (isRightStickDown)
		{
			Vec3 v1 = mPlayer->GetPos() - Camera::current.pos;
			Vec3 v2 = currentCamera_->GetLockonPos() - mPlayer->GetPos();

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

		float dis = Vec3(mBoss->GetPos() - mPlayer->GetPos()).Length();
		if (mPlayer->GetisAlive() == false ||
			mBoss->GetisAlive() == false || dis >= 200)
		{
			ChangeCamera(CameraManager::Default);
		}
	}

	//Camera::DebugCameraUpdate();
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

	nextCamera->Init(mPlayer);
	currentCamera_ = std::move(nextCamera);
}

CameraManager::CameraType CameraManager::GetCameraType()
{
	return cameraType_;
}

void CameraManager::SetPlayer(Player* player)
{
	mPlayer = player;
}

void CameraManager::SetBoss(Boss* boss)
{
	mBoss = boss;
}
