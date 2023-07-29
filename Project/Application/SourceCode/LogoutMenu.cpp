#include "LogoutMenu.h"

LogoutMenu::Select LogoutMenu::select_ = LogoutMenu::Select::BackToTitle;
bool LogoutMenu::isEnd_ = false;

LogoutMenu::LogoutMenu() :
	back_(std::make_unique<Sprite>()),
	textLight_(std::make_unique<Sprite>())
{
	back_->SetTexture(TextureManager::GetTexture("MenuBack"));
	textLight_->SetTexture(TextureManager::GetTexture("MenuTextLight"));
	textLight_->scale = 0.75f;

	for (uint32_t i = 0; i < textFrames_.size(); i++)
	{
		textFrames_[i] = std::make_unique<Sprite>();
		textFrames_[i]->SetTexture(TextureManager::GetTexture("MenuTextFrame"));

		Vec2 pos = GetWindowHalfSize() - Vec2::up * 80;
		Vec2 offset = Vec2::up * (float)i * 240;
		textFrames_[i]->pos = pos + offset;
		textFrames_[i]->scale = 0.75f;

		texts_[i] = std::make_unique<Sprite>();
		texts_[i]->pos = pos + offset;
		texts_[i]->scale = 0.35f;
	}

	texts_[0]->SetTexture(TextureManager::GetTexture("BackToTitleStr"));
	texts_[1]->SetTexture(TextureManager::GetTexture("CloseGameStr"));

	Init();
}

void LogoutMenu::Init()
{
	back_->pos = GetWindowHalfSize() + Vec2::up * 48;
	back_->color.a = 250.f;
	isEnd_ = false;
}

void LogoutMenu::Update()
{
	switch (select_)
	{
	case Select::BackToTitle:
		textLight_->pos = textFrames_[(uint32_t)Select::BackToTitle]->pos;

		if (Pad::GetButtonDown(PadCode::ButtonDown))
		{
			select_ = Select::CloseGame;
		}

		if (Pad::GetButtonDown(PadCode::ButtonA))
		{
			isEnd_ = true;
		}

		break;

	case Select::CloseGame:
		textLight_->pos = textFrames_[(uint32_t)Select::CloseGame]->pos;

		if (Pad::GetButtonDown(PadCode::ButtonUp))
		{
			select_ = Select::BackToTitle;
		}

		break;
	}

	if (Pad::GetButtonDown(PadCode::ButtonA))
	{
		isEnd_ = true;
	}

	back_->Update();
	textLight_->Update();
	for (uint32_t i = 0; i < textFrames_.size(); i++)
	{
		textFrames_[i]->Update();
		texts_[i]->Update();
	}
}

void LogoutMenu::DrawFrontSprite()
{
	back_->Draw();
	for (uint32_t i = 0; i < textFrames_.size(); i++)
	{
		textFrames_[i]->Draw();
		texts_[i]->Draw();
	}
	textLight_->Draw();
}

LogoutMenu::Select LogoutMenu::GetSelect()
{
	return select_;
}

bool LogoutMenu::GetisEnd()
{
	return isEnd_;
}
