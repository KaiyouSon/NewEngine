#include "TitleUI.h"

TitleUI::TitleUI() :
	backColor_(std::make_unique<Sprite>()),
	gameTitle_(std::make_unique<Sprite>()),
	pressButton_(std::make_unique<Sprite>())
{
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
	pressButton_->pos = Vec2(GetWindowHalfSize().x, 1080 - 256);
	pressButton_->scale = 0.25f;
}

void TitleUI::Update()
{
	backColor_->Update();
	gameTitle_->Update();
	pressButton_->Update();
}

void TitleUI::DrawFrontSprite()
{
	backColor_->Draw();
	gameTitle_->Draw();
	pressButton_->Draw();
}
