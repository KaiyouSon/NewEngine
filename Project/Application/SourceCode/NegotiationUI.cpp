#include "NegotiationUI.h"
#include "UIManager.h"

NegotiationUI::NegotiationUI() :
	mBackFrame(std::make_unique<Sprite>()),
	mButton(std::make_unique<Sprite>()),
	mColon(std::make_unique<Sprite>()),
	mText(std::make_unique<Sprite>()),
	mIsActive(false)
{
	mBackFrame->SetTexture(TextureManager::GetTexture("NegotiationBack"));

	mButton->SetTexture(TextureManager::GetTexture("Buttons"));
	mButton->SetTextureRect(Vec2(96, 96), Vec2(192, 192));
	mButton->SetSize(96);

	mColon->SetTexture(TextureManager::GetTexture("ColonStr"));

	mText->SetTexture(TextureManager::GetTexture("ReadMessageStr"));
}

void NegotiationUI::Init()
{
	const float height = 928.f;

	mBackFrame->pos = Vec2(GetWindowHalfSize().x, height);
	mBackFrame->scale = Vec2(1.25f, 0.75f);
	mBackFrame->color.a = 245.f;

	mButton->pos = Vec2(GetWindowHalfSize().x - 96, height);
	mButton->scale = 0.4f;

	mColon->pos = Vec2(GetWindowHalfSize().x - 70, height);
	mColon->scale = 0.3f;

	mText->pos = Vec2(GetWindowHalfSize().x + 32, height);
	mText->scale = 0.3f;
}

void NegotiationUI::Update()
{
	AlphaUpdate();
	TutorialMessageUpdate();

	mBackFrame->color.a = mAlpha - 10.f;
	mButton->color.a = mAlpha;
	mColon->color.a = mAlpha;
	mText->color.a = mAlpha;

	mBackFrame->Update();
	mButton->Update();
	mColon->Update();
	mText->Update();
}

void NegotiationUI::DrawFrontSprite()
{
	if (mAlpha == 0)
	{
		return;
	}

	mBackFrame->Draw();
	mButton->Draw();
	mColon->Draw();
	mText->Draw();
}

void NegotiationUI::AlphaUpdate()
{
	const float add = 20.f;

	if (mIsActive == false)
	{
		mAlpha -= add;
	}
	else
	{
		mAlpha += add;
	}
	mAlpha = Clamp<float>(mAlpha, 0.f, 255.f);
}

void NegotiationUI::TutorialMessageUpdate()
{
	if (mAlpha == 0)
	{
		return;
	}

	MessageUI* messageUI = mUiManager->GetMessageUI();

	if (mIsActive == false)
	{
		messageUI->SetisActive(false);
	}

	if (Pad::GetButtonDown(PadCode::ButtonB))
	{
		if (messageUI->GetisActive() == true)
		{
			messageUI->SetisActive(false);
		}
		else
		{
			messageUI->SetisActive(true);
		}
	}
}

void NegotiationUI::SetUIManager(UIManager* uiManager)
{
	mUiManager = uiManager;
}

void NegotiationUI::SetisActive(const bool isActive)
{
	mIsActive = isActive;
}

bool NegotiationUI::GetisActive()
{
	return mIsActive;
}
