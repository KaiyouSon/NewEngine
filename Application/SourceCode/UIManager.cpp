#include "UIManager.h"

UIManager::UIManager() :
	bossHPGauge_(std::make_unique<GaugeUI>())
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
}

void UIManager::Update()
{
	for (uint32_t i = 0; i < gauges_.size(); i++)
	{
		gauges_[i]->SetGaugePrame(player_->GetGaugeParam(i));
		gauges_[i]->Update();
	}

	bossHPGauge_->SetGaugePrame(boss_->GetHpGaugeParam());
	bossHPGauge_->Update();
}

void UIManager::DrawFrontSprite()
{
	for (uint32_t i = 0; i < gauges_.size(); i++)
	{
		gauges_[i]->DrawFrontSprite();
	}
	bossHPGauge_->DrawFrontSprite();
}

void UIManager::SetPlayer(Player* player)
{
	player_ = player;
}

void UIManager::SetBoss(Boss* boss)
{
	boss_ = boss;
}
