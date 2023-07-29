#pragma once
#include "IMenu.h"

class MenuManager
{
private:
	bool isActive_;
	std::vector<std::unique_ptr<IMenu>> menus_;

public:
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	bool GetisActive();
};

