#include "UIManager.h"

UIManager::UIManager() :
	bossHPGauge_(std::make_unique<GaugeUI>()),
	negotiationUI_(std::make_unique<NegotiationUI>())
{
	for (uint32_t i = 0; i < gauges_.size(); i++)
	{
		gauges_[i] = std::make_unique<GaugeUI>();
	}

	for (uint32_t i = 0; i < itemBoxUIs_.size(); i++)
	{
		itemBoxUIs_[i] = std::make_unique<ItemBoxUI>();
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

	// íÜêSç¿ïW
	itemBoxUIParent.pos = Vec2(192, GetWindowSize().y - 160);
	itemBoxUIParent.Update();
	for (uint32_t i = 0; i < gauges_.size(); i++)
	{
		itemBoxUIs_[i]->Init();
	}

	gauges_[(uint32_t)GaugeType::Hp]->SetColor(GaugeUI::FrontColor, Color::red);
	gauges_[(uint32_t)GaugeType::Mp]->SetColor(GaugeUI::FrontColor, Color::blue);
	gauges_[(uint32_t)GaugeType::Stamina]->SetColor(GaugeUI::FrontColor, Color::green);

	bossHPGauge_->SetGaugePrame(boss_->GetHpGaugeParam());
	bossHPGauge_->SetPos(Vec2(GetWindowHalfSize().x - 540.f, 880.f));
	bossHPGauge_->SetColor(GaugeUI::FrontColor, Color::red);
	bossHPGauge_->Init();

	negotiationUI_->Init();
}

void UIManager::Update()
{
	for (uint32_t i = 0; i < gauges_.size(); i++)
	{
		gauges_[i]->SetGaugePrame(player_->GetGaugeParam(i));
		gauges_[i]->Update();
	}

	float width = 92.f;
	float height = 56.f;
	itemBoxUIs_[0]->SetPos(Vec2(width, 0));	// ç∂
	itemBoxUIs_[1]->SetPos(Vec2(-width, 0));
	itemBoxUIs_[2]->SetPos(Vec2(0, height));
	itemBoxUIs_[3]->SetPos(Vec2(0, -height));

	for (uint32_t i = 0; i < itemBoxUIs_.size(); i++)
	{
		itemBoxUIs_[i]->Update(&itemBoxUIParent);
	}

	bossHPGauge_->SetGaugePrame(boss_->GetHpGaugeParam());
	bossHPGauge_->Update();

	negotiationUI_->Update();
}

void UIManager::DrawFrontSprite()
{
	for (uint32_t i = 0; i < gauges_.size(); i++)
	{
		gauges_[i]->DrawFrontSprite();
	}

	for (uint32_t i = 0; i < itemBoxUIs_.size(); i++)
	{
		itemBoxUIs_[i]->DrawFrontSprite();
	}

	negotiationUI_->DrawFrontSprite();

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
