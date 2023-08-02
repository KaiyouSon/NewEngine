#include "NegotiationUI.h"
#include "UIManager.h"

NegotiationUI::NegotiationUI() :
	backFrame_(std::make_unique<Sprite>()),
	button_(std::make_unique<Sprite>()),
	colon_(std::make_unique<Sprite>()),
	text_(std::make_unique<Sprite>()),
	isActive_(false)
{
	backFrame_->SetTexture(TextureManager::GetTexture("NegotiationBack"));

	button_->SetTexture(TextureManager::GetTexture("Buttons"));
	button_->SetTextureRect(Vec2(96, 96), Vec2(192, 192));
	button_->SetSize(96);

	colon_->SetTexture(TextureManager::GetTexture("ColonStr"));

	text_->SetTexture(TextureManager::GetTexture("ReadMessageStr"));
}

void NegotiationUI::Init()
{
	const float height = 928.f;

	backFrame_->pos = Vec2(GetWindowHalfSize().x, height);
	backFrame_->scale = Vec2(1.25f, 0.75f);
	backFrame_->color.a = 245.f;

	button_->pos = Vec2(GetWindowHalfSize().x - 96, height);
	button_->scale = 0.4f;

	colon_->pos = Vec2(GetWindowHalfSize().x - 70, height);
	colon_->scale = 0.3f;

	text_->pos = Vec2(GetWindowHalfSize().x + 32, height);
	text_->scale = 0.3f;
}

void NegotiationUI::Update()
{
	AlphaUpdate();
	TutorialMessageUpdate();

	backFrame_->color.a = alpha_ - 10.f;
	button_->color.a = alpha_;
	colon_->color.a = alpha_;
	text_->color.a = alpha_;

	backFrame_->Update();
	button_->Update();
	colon_->Update();
	text_->Update();
}

void NegotiationUI::DrawFrontSprite()
{
	if (alpha_ == 0)
	{
		return;
	}

	backFrame_->Draw();
	button_->Draw();
	colon_->Draw();
	text_->Draw();
}

void NegotiationUI::AlphaUpdate()
{
	const float add = 20.f;

	if (isActive_ == false)
	{
		alpha_ -= add;
	}
	else
	{
		alpha_ += add;
	}
	alpha_ = Clamp<float>(alpha_, 0.f, 255.f);
}

void NegotiationUI::TutorialMessageUpdate()
{
	if (alpha_ == 0)
	{
		return;
	}

	MessageUI* messageUI = mUiManager->GetMessageUI();

	if (isActive_ == false)
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
	isActive_ = isActive;
}

bool NegotiationUI::GetisActive()
{
	return isActive_;
}
