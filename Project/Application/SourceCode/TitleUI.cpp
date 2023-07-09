#include "TitleUI.h"

TitleUI::TitleUI() :
	backColor_(std::make_unique<Sprite>()),
	gameTitle_(std::make_unique<Sprite>()),
	pressButton_(std::make_unique<Sprite>()),
	pressButtonAfterImage_(std::make_unique<Sprite>())
{
	for (uint32_t i = 0; i < pressButtonBacks_.size(); i++)
	{
		pressButtonBacks_[i] = std::make_unique<Sprite>();
	}
}

void TitleUI::Init()
{
	backColor_->SetTexture(TextureManager::GetTexture("White"));
	backColor_->pos = GetWindowHalfSize();
	backColor_->color = Color::black;
	backColor_->SetSize(Vec2(1920, 1080));

	gameTitle_->SetTexture(TextureManager::GetTexture("GameTitle"));
	gameTitle_->pos = Vec2(GetWindowHalfSize().x, 320);
	gameTitle_->scale = 1.5f;

	pressButton_->SetTexture(TextureManager::GetTexture("PressButton"));
	pressButton_->pos = Vec2(GetWindowHalfSize().x, 880);
	pressButton_->scale = 0.25f;

	for (uint32_t i = 0; i < pressButtonBacks_.size(); i++)
	{
		pressButtonBacks_[i]->SetTexture(TextureManager::GetTexture("PressButtonBack"));
		pressButtonBacks_[i]->pos = Vec2(GetWindowHalfSize().x, 880);
	}
	pressButtonBacks_[0]->scale = Vec2(1.25f, 0.75f);
	pressButtonBacks_[0]->color = Color(0xf79e0f);
	pressButtonBacks_[1]->scale = Vec2(0.75f, 0.25f);
	pressButtonBacks_[1]->color = Color::white;

	pressButtonAfterImage_->SetTexture(TextureManager::GetTexture("PressButton"));
	pressButtonAfterImage_->pos = Vec2(GetWindowHalfSize().x, 880);
	pressButtonAfterImage_->scale = 0.25f;

	alphaEase_.SetEaseTimer(60);
	alphaEase_.SetPowNum(2);
	isReverce_ = false;
}

void TitleUI::Update()
{
	BackAlphaUpdate();
	AfterImagaUpdate();

	backColor_->Update();
	gameTitle_->Update();
	pressButton_->Update();
	for (uint32_t i = 0; i < pressButtonBacks_.size(); i++)
	{
		pressButtonBacks_[i]->Update();
	}

	if (isAfterImage_ == true)
	{
		pressButtonAfterImage_->Update();
	}
}

void TitleUI::DrawFrontSprite()
{
	backColor_->Draw();
	gameTitle_->Draw();

	for (uint32_t i = 0; i < pressButtonBacks_.size(); i++)
	{
		pressButtonBacks_[i]->Draw();
	}
	pressButton_->Draw();

	if (isAfterImage_ == true)
	{
		pressButtonAfterImage_->Draw();
	}
}

bool TitleUI::GetisEnd()
{
	return isEnd_;
}

void TitleUI::BackAlphaUpdate()
{
	float min = 55.f;
	alphaEase_.Update();
	if (isReverce_ == false)
	{
		for (uint32_t i = 0; i < pressButtonBacks_.size(); i++)
		{
			pressButtonBacks_[i]->color.a = alphaEase_.In(255.f, min);
		}
		if (alphaEase_.GetisEnd() == true)
		{
			isReverce_ = true;
			alphaEase_.Reset();
		}
	}
	else
	{
		for (uint32_t i = 0; i < pressButtonBacks_.size(); i++)
		{
			pressButtonBacks_[i]->color.a = alphaEase_.Out(min, 255.f);
		}
		if (alphaEase_.GetisEnd() == true)
		{
			isReverce_ = false;
			alphaEase_.Reset();
		}
	}
}

void TitleUI::AfterImagaUpdate()
{
	if (isAfterImage_ == false)
	{
		return;
	}

	if (afterImageStep_ == 0)
	{
		float max = 0.26f;
		pressButtonAfterImage_->scale += 0.001f;
		if (pressButtonAfterImage_->scale >= max)
		{
			afterImageStep_ = 1;
		}
	}
	else if (afterImageStep_ == 1)
	{
		float max = 0.265f;
		pressButtonAfterImage_->scale += 0.001f;
		if (pressButtonAfterImage_->scale >= max)
		{
			afterImageStep_ = 2;
		}

		if (afterImageEase_.GetisEnd() == true)
		{
			afterImageStep_ = 1;
			afterImageEase_.Reset();

			isAfterImage_ = false;
			isEnd_ = true;
		}

	}
	else if (afterImageStep_ == 2)
	{
		pressButtonAfterImage_->scale += 0.001f;
		pressButtonAfterImage_->color.a -= 10.f;
		if (pressButtonAfterImage_->color.a <= 0)
		{
			pressButtonAfterImage_->scale = 0.25f;
			pressButtonAfterImage_->color.a = 255.f;
			afterImageStep_ = 0;
			isAfterImage_ = false;
			isEnd_ = true;
		}
	}
}

void TitleUI::SetisAfterImage(const bool isAfterImage)
{
	isAfterImage_ = isAfterImage;
}
