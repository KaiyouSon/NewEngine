#pragma once
#include "NewEngine.h"

enum class MenuType
{
	LogoutMenu,
};

class IMenu
{
public:
	virtual ~IMenu() {}
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void DrawFrontSprite() = 0;
};

