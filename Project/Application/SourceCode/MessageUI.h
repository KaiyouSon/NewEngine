#pragma once
#include "NewEngine.h"

class MessageUI
{
private:
	std::unique_ptr<Sprite> mBack;
	std::unique_ptr<Sprite> mButton;
	std::unique_ptr<Sprite> mColon;
	std::unique_ptr<Sprite> mText;

	std::unique_ptr<Sprite> mMessage;
	std::unique_ptr<Sprite> mMessageSignUI;
	Transform mParent;

	float mAlpha;
	bool mIsActive;

public:
	MessageUI();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetisActive(const bool isActive);
	void SetTexture(Texture* texture);

public:
	bool GetisActive();
};

