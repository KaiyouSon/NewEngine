#include "AfterImage.h"

AfterImage::AfterImage()
{
	tag = "AfterImage";
}

void AfterImage::Init()
{
	mAfterImageStep = 0;

	mGameObject->isActive = false;
	mGameObject->scale = 0.3f;
	mGameObject->color.a = 255.f;
}

void AfterImage::Update()
{
	if (Key::GetKeyDown(DIK_SPACE))
	{
		mGameObject->isActive = true;
	}

	if (!mGameObject->isActive)
	{
		return;
	}

	const float accel = 0.002f;
	if (mAfterImageStep == 0)
	{
		float max = 0.3f;
		mGameObject->scale += accel;
		if (mGameObject->scale >= max)
		{
			mAfterImageStep = 1;
		}
	}
	else if (mAfterImageStep == 1)
	{
		float max = 0.35f;
		mGameObject->scale += accel;
		if (mGameObject->scale >= max)
		{
			mAfterImageStep = 2;
		}
	}
	else if (mAfterImageStep == 2)
	{
		mGameObject->scale += accel;
		mGameObject->color.a -= 10.f;
		if (mGameObject->color.a <= 0)
		{
			mAfterImageStep = 3;
		}
	}
}

std::shared_ptr<Script> AfterImage::CreateInstence()
{
	return std::make_shared<AfterImage>();
}
