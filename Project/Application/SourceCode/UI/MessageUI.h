#pragma once
#include "NewEngine.h"
#include "IUI.h"

// メッセージUIのクラス
class MessageUI :public IUI
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
	void Init() override;
	void Update() override;
	void Draw() override;

public:
	void SetTexture(Texture* texture);
};

