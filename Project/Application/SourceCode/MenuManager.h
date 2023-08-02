#pragma once
#include "IMenu.h"

class MenuManager
{
private:
	bool mIsActive;
	std::vector<std::unique_ptr<IMenu>> mMenus;

public:
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	bool GetisActive();
};

