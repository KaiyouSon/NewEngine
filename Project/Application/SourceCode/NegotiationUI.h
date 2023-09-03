#pragma once
#include "NewEngine.h"

class UIManager;

class NegotiationUI
{
public:
	enum StrType
	{
		ReadMessageStr,
		RestInLightStr,
		OpenStr,
	};

private:
	UIManager* mUiManager;
	std::unique_ptr<Sprite> mBackFrame;
	std::unique_ptr<Sprite> mButton;
	std::unique_ptr<Sprite> mText;
	std::unique_ptr<Sprite> mColon;

	float mAlpha;
	bool mIsActive;

	StrType mType;

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
	void SetStrType(const StrType type);

public:
	bool GetisActive();
};

