#include "AfterImage.h"

AfterImage::AfterImage()
{
	tag = "AfterImage";
}

void AfterImage::Init()
{
	obj = dynamic_cast<Sprite*>(mGameObject);
}

void AfterImage::Update()
{
	mGameObject->scale += 0.001f;
}

std::shared_ptr<Script> AfterImage::CreateInstence()
{
	return std::make_shared<AfterImage>();
}
