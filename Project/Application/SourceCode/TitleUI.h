#pragma once
#include "NewEngine.h"

class TitleUI
{
private:
	std::unique_ptr<Sprite> mBackColor;
	std::unique_ptr<Sprite> mGameTitle;
	std::unique_ptr<Sprite> mTitleLogo;
	std::unique_ptr<Sprite> mPressButton;
	std::array<std::unique_ptr<Sprite>, 2> mPressButtonBacks;
	std::unique_ptr<Sprite> mPressButtonAfterImage;

private:
	Easing mAlphaEase;
	bool mIsReverce;

private:
	uint32_t mAfterImageStep;
	Easing mAfterImageEase;
	bool mIsAfterImage;
	bool mIsEnd;

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
	bool GetisAfterImage();

public:
	void SetisAfterImage(const bool isAfterImage);
};

