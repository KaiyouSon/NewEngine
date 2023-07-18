#include "UIManager.h"

UIManager::UIManager() :
	bossHPGauge_(std::make_unique<GaugeUI>()),
	negotiationUI_(std::make_unique<NegotiationUI>()),
	tutorialUI_(std::make_unique<TutorialUI>()),
	itemBoxUIManager_(std::make_unique<ItemBoxUIManager>())

{
	for (uint32_t i = 0; i < gauges_.size(); i++)
	{
		gauges_[i] = std::make_unique<GaugeUI>();
	}
}

void UIManager::Init()
{
	for (uint32_t i = 0; i < gauges_.size(); i++)
	{
		gauges_[i]->SetGaugePrame(player_->GetGaugeParam(i));
		gauges_[i]->SetPos(Vec2(144.f, (float)(48.f + i * 18.f)));
		gauges_[i]->Init();
	}

	gauges_[(uint32_t)GaugeType::Hp]->SetColor(GaugeUI::FrontColor, Color::red);
	gauges_[(uint32_t)GaugeType::Mp]->SetColor(GaugeUI::FrontColor, Color::blue);
	gauges_[(uint32_t)GaugeType::Stamina]->SetColor(GaugeUI::FrontColor, Color::green);

	bossHPGauge_->SetGaugePrame(boss_->GetHpGaugeParam());
	bossHPGauge_->SetPos(Vec2(GetWindowHalfSize().x - 540.f, 880.f));
	bossHPGauge_->SetColor(GaugeUI::FrontColor, Color::red);
	bossHPGauge_->Init();

	negotiationUI_->Init();
	negotiationUI_->SetUIManager(this);

	tutorialUI_->Init();

	itemBoxUIManager_->Init();
	notActiveTimer_.SetLimitTimer(180);
}

void UIManager::Update()
{
	bool isLeftStickMove = Pad::GetStick(PadCode::LeftStick, 300) != 0;
	bool isAnyButtonDown = Pad::GetAnyButtonDown();

	if (isLeftStickMove || isAnyButtonDown)
	{
		notActiveTimer_.Reset();
		isActive_ = true;
	}
	else
	{
		notActiveTimer_.Update(false);
		if (notActiveTimer_.GetisTimeOut() == true)
		{
			isActive_ = false;
		}
	}

	float speed = 15;
	if (isActive_ == true)
	{
		alpha_ += speed;
	}
	else
	{
		alpha_ -= speed;
	}
	alpha_ = Clamp<float>(alpha_, 0, 255);

	// アイテムボックス(左下のやつ)
	itemBoxUIManager_->SetAlpha(alpha_);
	itemBoxUIManager_->Update();

	// プレイヤーのゲージ
	for (uint32_t i = 0; i < gauges_.size(); i++)
	{
		gauges_[i]->SetGaugePrame(player_->GetGaugeParam(i));
		gauges_[i]->Update();
	}

	bossHPGauge_->SetGaugePrame(boss_->GetHpGaugeParam());
	bossHPGauge_->Update();

	negotiationUI_->Update();

	tutorialUI_->Update();

}

void UIManager::DrawFrontSprite()
{
	for (uint32_t i = 0; i < gauges_.size(); i++)
	{
		gauges_[i]->DrawFrontSprite();
	}

	negotiationUI_->DrawFrontSprite();

	tutorialUI_->DrawFrontSprite();

	itemBoxUIManager_->DrawFrontSprite();

	//bossHPGauge_->DrawFrontSprite();
}

void UIManager::SetPlayer(Player* player)
{
	player_ = player;
}

void UIManager::SetBoss(Boss* boss)
{
	boss_ = boss;
}

NegotiationUI* UIManager::GetNegotiationUI()
{
	return negotiationUI_.get();
}

TutorialUI* UIManager::GetTutorialUI()
{
	return tutorialUI_.get();
}

bool UIManager::GetisActive()
{
	isActive_ = false;



	return isActive_;
}
