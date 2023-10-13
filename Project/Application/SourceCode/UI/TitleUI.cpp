#include "TitleUI.h"

TitleUI::TitleUI() :
	mBackColor(std::make_unique<Sprite>()),
	mGameTitle(std::make_unique<Sprite>()),
	mTitleLogo(std::make_unique<Sprite>()),
	mPressButton(std::make_unique<Sprite>()),
	mPressButtonAfterImage(std::make_unique<Sprite>())
{
	for (uint32_t i = 0; i < mPressButtonBacks.size(); i++)
	{
		mPressButtonBacks[i] = std::make_unique<Sprite>();
	}
}

void TitleUI::Init()
{
	mBackColor->SetTexture(TextureManager::GetTexture("White"));
	mBackColor->pos = GetWindowHalfSize();
	mBackColor->color = Color::black;
	mBackColor->SetSize(Vec2(1920, 1080));

	mGameTitle->SetTexture(TextureManager::GetTexture("GameTitle"));
	mGameTitle->pos = Vec2(GetWindowHalfSize().x, 320);
	mGameTitle->scale = 1.75f;

	mTitleLogo->SetTexture(TextureManager::GetTexture("TitleLogo"));
	mTitleLogo->pos = Vec2(GetWindowHalfSize().x, GetWindowHalfSize().y - 150);
	mTitleLogo->scale = 1.25f;

	mPressButton->SetTexture(TextureManager::GetTexture("PressButton"));
	mPressButton->pos = Vec2(GetWindowHalfSize().x, 900);
	mPressButton->scale = 0.3f;

	for (uint32_t i = 0; i < mPressButtonBacks.size(); i++)
	{
		mPressButtonBacks[i]->SetTexture(TextureManager::GetTexture("PressButtonBack"));
		mPressButtonBacks[i]->pos = Vec2(GetWindowHalfSize().x, 900);
	}
	mPressButtonBacks[0]->scale = Vec2(1.5f, 0.8f);
	mPressButtonBacks[0]->color = Color(0xf79e0f);
	mPressButtonBacks[1]->scale = Vec2(1.f, 0.3f);
	mPressButtonBacks[1]->color = Color::white;

	mPressButtonAfterImage->SetTexture(TextureManager::GetTexture("PressButton"));
	mPressButtonAfterImage->pos = Vec2(GetWindowHalfSize().x, 900);
	mPressButtonAfterImage->scale = 0.3f;

	mAlphaEase.SetEaseTimer(60);
	mAlphaEase.SetPowNum(2);
	mIsReverce = false;

	mIsAfterImage = false;
	mAfterImageStep = 0;
}

void TitleUI::Update()
{
	BackAlphaUpdate();
	AfterImagaUpdate();

	mTitleLogo->pos = Vec2(GetWindowHalfSize().x, GetWindowHalfSize().y - 100);

	mBackColor->Update();
	mGameTitle->Update();
	mTitleLogo->Update();
	mPressButton->Update();
	for (uint32_t i = 0; i < mPressButtonBacks.size(); i++)
	{
		mPressButtonBacks[i]->Update();
	}

	if (mIsAfterImage == true)
	{
		mPressButtonAfterImage->Update();
	}
}

void TitleUI::DrawFrontSprite()
{
	mBackColor->Draw();
	mTitleLogo->Draw();
	mGameTitle->Draw();

	for (uint32_t i = 0; i < mPressButtonBacks.size(); i++)
	{
		mPressButtonBacks[i]->Draw();
	}
	mPressButton->Draw();

	if (mIsAfterImage == true)
	{
		mPressButtonAfterImage->Draw();
	}
}

bool TitleUI::GetisEnd()
{
	return mIsEnd;
}

bool TitleUI::GetisAfterImage()
{
	return mIsAfterImage;
}

void TitleUI::BackAlphaUpdate()
{
	float min = 55.f;
	mAlphaEase.Update();
	if (mIsReverce == false)
	{
		for (uint32_t i = 0; i < mPressButtonBacks.size(); i++)
		{
			mPressButtonBacks[i]->color.a = mAlphaEase.In(255.f, min);
		}
		if (mAlphaEase.GetisEnd() == true)
		{
			mIsReverce = true;
			mAlphaEase.Reset();
		}
	}
	else
	{
		for (uint32_t i = 0; i < mPressButtonBacks.size(); i++)
		{
			mPressButtonBacks[i]->color.a = mAlphaEase.Out(min, 255.f);
		}
		if (mAlphaEase.GetisEnd() == true)
		{
			mIsReverce = false;
			mAlphaEase.Reset();
		}
	}
}

void TitleUI::AfterImagaUpdate()
{
	if (mIsAfterImage == false)
	{
		return;
	}

	const float accel = 0.002f;

	if (mAfterImageStep == 0)
	{
		float max = 0.3f;
		mPressButtonAfterImage->scale += accel;
		if (mPressButtonAfterImage->scale >= max)
		{
			mAfterImageStep = 1;
		}
	}
	else if (mAfterImageStep == 1)
	{
		float max = 0.35f;
		mPressButtonAfterImage->scale += accel;
		if (mPressButtonAfterImage->scale >= max)
		{
			mAfterImageStep = 2;
		}

		//if (afterImageEase_.GetisEnd() == true)
		//{
		//	mAfterImageStep = 1;
		//	afterImageEase_.Reset();
		//	mIsEnd = true;
		//}

	}
	else if (mAfterImageStep == 2)
	{
		mPressButtonAfterImage->scale += accel;
		mPressButtonAfterImage->color.a -= 10.f;
		if (mPressButtonAfterImage->color.a <= 0)
		{
			//mPressButtonAfterImage->scale = 0.425f;
			//mPressButtonAfterImage->color.a = 255.f;
			mAfterImageStep = 2;
			mIsEnd = true;
		}
	}
}

void TitleUI::SetisAfterImage(const bool isAfterImage)
{
	mIsAfterImage = isAfterImage;
}