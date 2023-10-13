#include "CameraManager.h"
#include "DefaultCamera.h"
#include "TargetCamera.h"
#include "MovieCamera.h"

CameraManager::CameraManager()
{
}

void CameraManager::Init()
{
	mCameraType = CameraType::Default;
	mCurrentCamera = std::make_unique<DefaultCamera>();
	mCurrentCamera->Init(mPlayer);
}

void CameraManager::Update()
{
	mCurrentCamera->SetLockonPos(mBoss->GetPos());
	mCurrentCamera->Update();

	if (Camera::current.rot.y >= Radian(360))
	{
		Camera::current.rot.y -= Radian(360);
	}
	else if (Camera::current.rot.y <= -Radian(360))
	{
		Camera::current.rot.y += Radian(360);
	}

	// 繧ｫ繝｡繝ｩ蛻・ｊ譖ｿ縺医ｋ蜃ｦ逅・
	bool isRightStickDown = Pad::GetButtonDown(PadCode::RightStick);
	if (mCameraType == CameraType::Default)
	{
		if (isRightStickDown)
		{
			Vec3 v1 = mPlayer->GetPos() - Camera::current.pos;
			Vec3 v2 = mCurrentCamera->GetLockonPos() - mPlayer->GetPos();

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
					mCurrentCamera->SetisEase(true);
				}
			}
		}
	}
	else if (mCameraType == CameraType::Target)
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
	mCameraType = cameraType;

	std::unique_ptr<ICamera> nextCamera;
	switch (mCameraType)
	{
	case CameraType::Default:
		nextCamera = std::make_unique<DefaultCamera>();
		break;

	case CameraType::Target:
		nextCamera = std::make_unique<TargetCamera>();
		break;

	case CameraType::Movie:
		nextCamera = std::make_unique<MovieCamera>();
		break;

	default:
		break;
	}

	nextCamera->Init(mPlayer);
	mCurrentCamera = std::move(nextCamera);
}

CameraManager::CameraType CameraManager::GetCameraType()
{
	return mCameraType;
}

void CameraManager::SetPlayer(Player* player)
{
	mPlayer = player;
}

void CameraManager::SetBoss(Boss* boss)
{
	mBoss = boss;
}

