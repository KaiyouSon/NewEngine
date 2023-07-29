#include "MenuManager.h"
#include "LogoutMenu.h"

void MenuManager::Update()
{
	if (Pad::GetButtonDown(PadCode::ButtonStart))
	{
		if (isActive_ == false)
		{
			menus_.emplace_back(std::move(std::make_unique<LogoutMenu>()));
			isActive_ = true;
		}
		else
		{
			menus_.clear();
			isActive_ = false;
		}
	}

	if (menus_.empty() == false)
	{
		menus_.back()->Update();
	}
}

void MenuManager::DrawFrontSprite()
{
	for (const auto& cur : menus_)
	{
		cur->DrawFrontSprite();
	}
}
