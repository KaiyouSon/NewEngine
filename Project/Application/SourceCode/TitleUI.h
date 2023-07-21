#pragma once
#include "NewEngine.h"

class TitleUI
{
private:
	std::unique_ptr<Sprite> backColor_;
	std::unique_ptr<Sprite> gameTitle_;
	std::unique_ptr<Sprite> titleLogo_;
	std::unique_ptr<Sprite> pressButton_;
	std::array<std::unique_ptr<Sprite>, 2> pressButtonBacks_;
	std::unique_ptr<Sprite> pressButtonAfterImage_;

private:
	Easing alphaEase_;
	bool isReverce_;

private:
	uint32_t afterImageStep_;
	Easing afterImageEase_;
	bool isAfterImage_;
	bool isEnd_;

private:
	void BackAlphaUpdate();
	void AfterImagaUpdate();

public:
	TitleUI();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	bool GetisEnd();

public:
	void SetisAfterImage(const bool isAfterImage);
};

