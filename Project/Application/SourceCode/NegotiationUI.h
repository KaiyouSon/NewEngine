#pragma once
#include "NewEngine.h"

class UIManager;

class NegotiationUI
{
private:
	UIManager* mUiManager;
	std::unique_ptr<Sprite> mBackFrame;
	std::unique_ptr<Sprite> mButton;
	std::unique_ptr<Sprite> mText;
	std::unique_ptr<Sprite> mColon;

	float mAlpha;
	bool mIsActive;

private:
	void AlphaUpdate();
	void TutorialMessageUpdate();

public:
	NegotiationUI();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetUIManager(UIManager* uiManager);
	void SetisActive(const bool isActive);

public:
	bool GetisActive();
};

