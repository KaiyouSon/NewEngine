#include "TutorialUI.h"

TutorialUI::TutorialUI() :
	back_(std::make_unique<Sprite>()),
	button_(std::make_unique<Sprite>()),
	colon_(std::make_unique<Sprite>()),
	text_(std::make_unique<Sprite>()),
	messageSignUI_(std::make_unique<Sprite>()),
	message_(std::make_unique<Sprite>())
{
	back_->SetTexture(TextureManager::GetTexture("MessageBack"));
	button_->SetTexture(TextureManager::GetTexture("Buttons"));
	colon_->SetTexture(TextureManager::GetTexture("ColonStr"));
	text_->SetTexture(TextureManager::GetTexture("ExitStr"));
	messageSignUI_->SetTexture(TextureManager::GetTexture("MessageSignUI"));
	message_->SetTexture(TextureManager::GetTexture("TutorialStr1"));

	button_->SetTextureRect(Vec2(96, 96), Vec2(192, 192));
	button_->SetSize(96);
}

void TutorialUI::Init()
{
	parent_.pos = Vec2(GetWindowHalfSize().x, 224);
	parent_.Update();

	float height = 72.f;
	button_->pos = Vec2(-40, height);
	colon_->pos = Vec2(-20, height);
	text_->pos = Vec2(20, height);
	message_->pos = Vec2(-150.f, -20.f);

	button_->scale = 0.4f;
	colon_->scale = 0.3f;
	text_->scale = 0.3f;
	message_->scale = 0.35f;
}

void TutorialUI::Update()
{
	const float add = isActive_ ? 35.f : -35.f;
	alpha_ += add;
	alpha_ = Clamp<float>(alpha_, 0.f, 255.f);

	messageSignUI_->scale = 0.4f;
	messageSignUI_->pos.x = -350.f;

	back_->color.a = alpha_;
	button_->color.a = alpha_;
	colon_->color.a = alpha_;
	text_->color.a = alpha_;

	back_->Update(&parent_);
	button_->Update(&parent_);
	colon_->Update(&parent_);
	text_->Update(&parent_);
	messageSignUI_->Update(&parent_);
	message_->Update(&parent_);
}

void TutorialUI::DrawFrontSprite()
{
	if (alpha_ == 0)
	{
		return;
	}

	back_->Draw();
	button_->Draw();
	colon_->Draw();
	text_->Draw();
	messageSignUI_->Draw();
	message_->Draw();
}

void TutorialUI::SetisActive(const bool isActive)
{
	isActive_ = isActive;
}

bool TutorialUI::GetisActive()
{
	return isActive_;
}
