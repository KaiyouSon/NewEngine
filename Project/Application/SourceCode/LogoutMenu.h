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
	std::unique_ptr<Sprite> back_;
	std::unique_ptr<Sprite> textLight_;
	std::array<std::unique_ptr<Sprite>, 2> textFrames_;
	std::array<std::unique_ptr<Sprite>, 2> texts_;

	static Select select_;
	static bool isEnd_;

public:
	LogoutMenu();
	void Init() override;
	void Update() override;
	void DrawFrontSprite() override;

public:
	static Select GetSelect();
	static bool GetisEnd();
};

