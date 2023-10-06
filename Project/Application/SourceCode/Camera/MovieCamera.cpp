#include "MovieCamera.h"

MovieEvent* MovieCamera::sMovieEvent = nullptr;

MovieCamera::MovieCamera()
{
}

void MovieCamera::Init(Player* player)
{
	mPlayer = player;

	//mCamera->pos = Vec3(45.f, 1.5f, 20.f);
	//mCamera->rot = Radian(Vec3(-90, -90, 0));

	mCamera->pos = Vec3(-65.f, 75.f, -85.f);
	mCamera->rot = Radian(Vec3(25, 40, 0));
}

void MovieCamera::Update()
{
	if (sMovieEvent->GetPlayStep() == MovieEvent::PlayStep::CameraZoom)
	{
		mFrontVec = mPlayer->GetPos() - mCamera->pos;
		mCamera->pos += mFrontVec.Norm() * 0.1f;
	}
	else if (sMovieEvent->GetPlayStep() == MovieEvent::PlayStep::OpenCoffin)
	{
		mCamera->pos = Vec3(50.f, 20.f, 0);
		mCamera->rot = Radian(Vec3(40, -30, 0));
	}

	// Œ»İ‚ÌƒJƒƒ‰‚É‘ã“ü
	Camera::current = *mCamera;
}

void MovieCamera::SetMovieEvent(MovieEvent* movieEvent)
{
	sMovieEvent = movieEvent;
}
