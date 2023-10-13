#pragma once
#include "IMenu.h"
#include "MovieEvent.h"

class MenuManager
{
private:
	bool mIsActive;
	std::vector<std::unique_ptr<IMenu>> mMenus;

private:
	MovieEvent* mMovieEvent;

public:
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetMovieEvent(MovieEvent* movieEvent);

public:
	bool GetisActive();
};
