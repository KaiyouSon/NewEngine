#include "AfterImage.h"

AfterImage::AfterImage()
{
	tag = "AfterImage";
}

void AfterImage::Init()
{
}

void AfterImage::Update()
{
	//if (Key::GetKeyDown(DIK_SPACE))
	//{
	//	isStart = true;
	//}

	//if (!isStart)
	//{
	//	return;
	//}

	mGameObject->scale += 0.01f;
	if (mGameObject->scale >= 3.f)
	{
		mGameObject->color.a--;
	}
}

std::shared_ptr<Script> AfterImage::CreateInstence()
{
	return std::make_shared<AfterImage>();
}
