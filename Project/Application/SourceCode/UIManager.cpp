#include "UIManager.h"

UIManager::UIManager() :
	bossHPGauge_(std::make_unique<GaugeUI>()),
	negotiationUI_(std::make_unique<NegotiationUI>()),
	messageUI_(std::make_unique<MessageUI>()),
	itemBoxUIManager_(std::make_unique<ItemBoxUIManager>()),
	resultUI_(std::make_unique<ResultUI>())
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
	bossHPGauge_->SetPos(Vec2(GetWindowHalfSize().x - 448.f, 880.f));
	bossHPGauge_->SetColor(GaugeUI::FrontColor, Color::red);
	bossHPGauge_->Init();

	negotiationUI_->Init();
	negotiationUI_->SetUIManager(this);

	messageUI_->Init();

	itemBoxUIManager_->Init();
	itemBoxUIManager_->SetPlayer(player_);

	resultUI_->Init();
}

void UIManager::Update()
{
	// デバッグ時のみ実行
	ProcessAtDebugBulid([&]()
		{
			if (Key::GetKeyDown(DIK_1))
			{
				resultUI_->SetisActive(true);
				resultUI_->SetResultType(ResultUI::ResultType::EnemyFelledStr);
			}
			else if (Key::GetKeyDown(DIK_2))
			{
				resultUI_->SetisActive(true);
				resultUI_->SetResultType(ResultUI::ResultType::YouDiedStr);
			}
		});

	if (player_->GetisDissolve() == true)
	{
		resultUI_->SetisActive(true);
		resultUI_->SetResultType(ResultUI::ResultType::YouDiedStr);
	}
	else if (boss_->GetisDissolve() == true)
	{
		resultUI_->SetisActive(true);
		resultUI_->SetResultType(ResultUI::ResultType::EnemyFelledStr);
	}

	// アイテムボックス(左下のやつ)
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

	messageUI_->Update();

	resultUI_->Update();
}

void UIManager::DrawFrontSprite()
{
	for (uint32_t i = 0; i < gauges_.size(); i++)
	{
		gauges_[i]->DrawFrontSprite();
	}

	negotiationUI_->DrawFrontSprite();

	messageUI_->DrawFrontSprite();

	itemBoxUIManager_->DrawFrontSprite();

	bossHPGauge_->DrawFrontSprite();

	resultUI_->DrawFrontSprite();
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

MessageUI* UIManager::GetMessageUI()
{
	return messageUI_.get();
}
