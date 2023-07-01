#include "UIManager.h"

UIManager::UIManager()
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
		gauges_[i]->Init();
		gauges_[i]->SetPos(Vec2(144.f, (float)(48.f + i * 18.f)));
	}

	gauges_[(uint32_t)GaugeType::Hp]->SetColor(GaugeUI::FrontColor, Color::red);
	gauges_[(uint32_t)GaugeType::Mp]->SetColor(GaugeUI::FrontColor, Color::blue);
	gauges_[(uint32_t)GaugeType::Stamina]->SetColor(GaugeUI::FrontColor, Color::green);
}

void UIManager::Update()
{
	for (uint32_t i = 0; i < gauges_.size(); i++)
	{
		gauges_[i]->Update();
	}
}

void UIManager::DrawFrontSprite()
{
	for (uint32_t i = 0; i < gauges_.size(); i++)
	{
		gauges_[i]->DrawFrontSprite();
	}
}

void UIManager::SetPlayer(Player* player)
{
	player_ = player;
}
