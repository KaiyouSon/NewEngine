#include "MenuManager.h"
#include "LogoutMenu.h"

void MenuManager::Init()
{
	LogoutMenu::SetisEnd(false);
}

void MenuManager::Update()
{
	if (mMovieEvent->GetisPlaying() == true)
	{
		return;
	}

	if (Pad::GetButtonDown(PadCode::ButtonStart))
	{
		SoundManager::Play("SelectSE");
		if (mIsActive == false)
		{
			mMenus.emplace_back(std::move(std::make_unique<LogoutMenu>()));
			mIsActive = true;
		}
		else
		{
			mMenus.clear();
			mIsActive = false;
		}
	}

	if (mMenus.empty() == false)
	{
		mMenus.back()->Update();
	}
}

void MenuManager::DrawFrontSprite()
{
	if (mMovieEvent->GetisPlaying() == true)
	{
		return;
	}

	for (const auto& cur : mMenus)
	{
		cur->DrawFrontSprite();
	}
}

void MenuManager::SetMovieEvent(MovieEvent* movieEvent)
{
	mMovieEvent = movieEvent;
}

bool MenuManager::GetisActive()
{
	return mIsActive;
}
