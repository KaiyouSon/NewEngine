#include "LogoutMenu.h"

LogoutMenu::Select LogoutMenu::sSelect = LogoutMenu::Select::BackToTitle;
bool LogoutMenu::sIsEnd = false;

LogoutMenu::LogoutMenu() :
	mBack(std::make_unique<Sprite>()),
	mTextLight(std::make_unique<Sprite>())
{
	mBack->SetTexture(TextureManager::GetTexture("MenuBack"));
	mTextLight->SetTexture(TextureManager::GetTexture("MenuTextLight"));
	mTextLight->scale = 0.75f;

	for (uint32_t i = 0; i < mTextFrames.size(); i++)
	{
		mTextFrames[i] = std::make_unique<Sprite>();
		mTextFrames[i]->SetTexture(TextureManager::GetTexture("MenuTextFrame"));

		Vec2 pos = GetWindowHalfSize() - Vec2::up * 80;
		Vec2 offset = Vec2::up * (float)i * 240;
		mTextFrames[i]->pos = pos + offset;
		mTextFrames[i]->scale = 0.75f;

		mTexts[i] = std::make_unique<Sprite>();
		mTexts[i]->pos = pos + offset;
		mTexts[i]->scale = 0.35f;
	}

	mTexts[0]->SetTexture(TextureManager::GetTexture("BackToTitleStr"));
	mTexts[1]->SetTexture(TextureManager::GetTexture("CloseGameStr"));

	Init();
}

void LogoutMenu::Init()
{
	mBack->pos = GetWindowHalfSize() + Vec2::up * 48;
	mBack->color.a = 250.f;
	sIsEnd = false;
}

void LogoutMenu::Update()
{
	const float deadZoneY = 300;

	switch (sSelect)
	{
	case Select::BackToTitle:
		mTextLight->pos = mTextFrames[(uint32_t)Select::BackToTitle]->pos;

		if (Pad::GetButtonDown(PadCode::ButtonDown) ||
			Pad::GetStickDown(PadCode::LeftStick, deadZoneY).y > 0)
		{
			SoundManager::Play("SelectSE");
			sSelect = Select::CloseGame;
		}

		if (Pad::GetButtonDown(PadCode::ButtonA))
		{
			sIsEnd = true;
		}

		break;

	case Select::CloseGame:
		mTextLight->pos = mTextFrames[(uint32_t)Select::CloseGame]->pos;

		if (Pad::GetButtonDown(PadCode::ButtonUp) ||
			Pad::GetStickDown(PadCode::LeftStick, deadZoneY).y < 0)
		{
			SoundManager::Play("SelectSE");
			sSelect = Select::BackToTitle;
		}

		break;
	}

	if (Pad::GetButtonDown(PadCode::ButtonA))
	{
		SoundManager::Play("SelectSE");
		sIsEnd = true;
	}

	mBack->Update();
	mTextLight->Update();
	for (uint32_t i = 0; i < mTextFrames.size(); i++)
	{
		mTextFrames[i]->Update();
		mTexts[i]->Update();
	}
}

void LogoutMenu::DrawFrontSprite()
{
	mBack->Draw();
	for (uint32_t i = 0; i < mTextFrames.size(); i++)
	{
		mTextFrames[i]->Draw();
		mTexts[i]->Draw();
	}
	mTextLight->Draw();
}

LogoutMenu::Select LogoutMenu::GetSelect()
{
	return sSelect;
}

bool LogoutMenu::GetisEnd()
{
	return sIsEnd;
}

void LogoutMenu::SetisEnd(const bool isEnd)
{
	sIsEnd = isEnd;
}
