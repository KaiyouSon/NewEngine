#include "MessageUI.h"

MessageUI::MessageUI() :
	mBack(std::make_unique<Sprite>()),
	mButton(std::make_unique<Sprite>()),
	mColon(std::make_unique<Sprite>()),
	mText(std::make_unique<Sprite>()),
	mMessageSignUI(std::make_unique<Sprite>()),
	mMessage(std::make_unique<Sprite>())
{
	mBack->SetTexture("MessageBack");
	mButton->SetTexture("Buttons");
	mColon->SetTexture("ColonStr");
	mText->SetTexture("ExitStr");
	mMessageSignUI->SetTexture("MessageSignUI");
	mMessage->SetTexture("TutorialStr1");

	mMessage->SetAnchorPoint(Vec2(0.0f, 0.5f));

	mButton->SetTextureRect(Vec2(96, 96), Vec2(192, 192));
	mButton->SetSize(96);
}

void MessageUI::Init()
{
	mParent.pos = Vec2(GetWindowHalfSize().x, 224);
	mParent.Update();

	float height = 72.f;
	mButton->pos = Vec2(-40, height);
	mColon->pos = Vec2(-20, height);
	mText->pos = Vec2(20, height);
	mMessage->pos = Vec2(-150.f, -20.f);

	mButton->scale = 0.4f;
	mColon->scale = 0.3f;
	mText->scale = 0.3f;
	mMessage->scale = 0.35f;
}

void MessageUI::Update()
{
	const float add = mIsActive ? 35.f : -35.f;
	mAlpha += add;
	mAlpha = Clamp<float>(mAlpha, 0.f, 255.f);

	mMessage->pos = Vec2(-250.f, -20.f);

	mMessageSignUI->scale = 0.4f;
	mMessageSignUI->pos.x = -350.f;

	mBack->color.a = mAlpha;
	mButton->color.a = mAlpha;
	mColon->color.a = mAlpha;
	mText->color.a = mAlpha;

	mBack->SetParent(&mParent);
	mButton->SetParent(&mParent);
	mColon->SetParent(&mParent);
	mText->SetParent(&mParent);
	mMessageSignUI->SetParent(&mParent);
	mMessage->SetParent(&mParent);

	mBack->Update();
	mButton->Update();
	mColon->Update();
	mText->Update();
	mMessageSignUI->Update();
	mMessage->Update();
}

void MessageUI::Draw()
{
	if (mAlpha == 0)
	{
		return;
	}

	mBack->Draw("FrontSprite");
	mButton->Draw("FrontSprite");
	mColon->Draw("FrontSprite");
	mText->Draw("FrontSprite");
	mMessageSignUI->Draw("FrontSprite");
	mMessage->Draw("FrontSprite");
}

void MessageUI::SetTexture(Texture* texture)
{
	mMessage->SetTexture(texture->GetTag());
}