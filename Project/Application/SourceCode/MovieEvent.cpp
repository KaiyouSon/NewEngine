#include "MovieEvent.h"
#include "Player.h"
#include "CameraManager.h"
#include "TransitionManager.h"

bool MovieEvent::sIsPlayOnce = false;

void MovieEvent::Init()
{
	mStep = Step::None;

	mTimer.SetLimitTimer(240);
	mTimer.Reset();
}

void MovieEvent::Update()
{
	switch (mStep)
	{
	case Step::Start:
		mPlayer->SetPos(Vec3(41.f, 1.f, 20.f));
		mPlayer->SetRot(Radian(Vec3(-90, -90, 0)));

		mStartPos = mPlayerCoffin->GetTopPos();
		mEndPos = mPlayerCoffin->GetTopPos() - Vec3(0, 0, 4);

		mStep = Step::Play;

		break;

	case Step::Play:
		PlayMovieUpdate();
		break;
	}
}

void MovieEvent::Start()
{
	if (sIsPlayOnce == true)
	{
		return;
	}

	if (mStep != Step::None)
	{
		return;
	}

	mStep = Step::Start;
	mPlayStep = PlayStep::CameraZoom;

	mEase.Reset();
	mEase.SetEaseTimer(360);
	mEase.SetPowNum(3);
	sIsPlayOnce = true;

	CameraManager::GetInstance()->ChangeCamera(CameraManager::CameraType::Movie);
}

void MovieEvent::End()
{
	mStep = Step::End;
	Init();
}

void MovieEvent::PlayMovieUpdate()
{
	switch (mPlayStep)
	{
	case PlayStep::CameraZoom:
		// カメラの処理はMovieCameraの方で処理をしている
		mTimer.Update();
		if (mTimer.GetisTimeOut() == true)
		{
			mPlayStep = PlayStep::OpenCoffin;
		}
		break;


	case PlayStep::OpenCoffin:
	{
		Vec3 pos = mEase.InOut(mStartPos, mEndPos);
		mPlayerCoffin->SetTopPos(pos);
		mEase.Update();
		if (mEase.GetisEnd() == true)
		{
			TransitionManager::GetInstance()->Start(TransitionType::Scene);
			mPlayStep = PlayStep::End;
		}
	}
	break;

	case PlayStep::End:
		break;
	}
}

void MovieEvent::SetPlayer(Player* player)
{
	mPlayer = player;
}

void MovieEvent::SetPlayerCoffin(Coffin* playerCoffin)
{
	mPlayerCoffin = playerCoffin;
}

void MovieEvent::SetisPlayOnce(const bool isPlayOnce)
{
	sIsPlayOnce = isPlayOnce;
}

bool MovieEvent::GetisPlaying()
{
	return mStep != Step::None && mStep != Step::End;
}

MovieEvent::PlayStep MovieEvent::GetPlayStep()
{
	return mPlayStep;
}
