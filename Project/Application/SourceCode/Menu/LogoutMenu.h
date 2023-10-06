#pragma once
#include "IMenu.h"

class LogoutMenu : public IMenu
{
public:
	enum Select
	{
		BackToTitle,
		CloseGame,
	};

private:
	std::unique_ptr<Sprite> mBack;
	std::unique_ptr<Sprite> mTextLight;
	std::array<std::unique_ptr<Sprite>, 2> mTextFrames;
	std::array<std::unique_ptr<Sprite>, 2> mTexts;

	static Select sSelect;
	static bool sIsEnd;

public:
	LogoutMenu();
	void Init() override;
	void Update() override;
	void DrawFrontSprite() override;

public:
	static Select GetSelect();
	static bool GetisEnd();

	static void SetisEnd(const bool isEnd);
};

