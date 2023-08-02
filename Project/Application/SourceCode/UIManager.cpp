#include "UIManager.h"

UIManager::UIManager() :
	bossHPGauge_(std::make_unique<GaugeUI>()),
	negotiationUI_(std::make_unique<NegotiationUI>()),
	messageUI_(std::make_unique<MessageUI>()),
	itemBoxmUiManager(std::make_unique<ItemBoxUIManager>()),
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
		gauges_[i]->SetGaugePrame(mPlayer->GetGaugeParam(i));
		gauges_[i]->SetPos(Vec2(144.f, (float)(48.f + i * 18.f)));
		gauges_[i]->Init();
	}

	gauges_[(uint32_t)GaugeType::Hp]->SetColor(GaugeUI::FrontColor, Color::red);
	gauges_[(uint32_t)GaugeType::Mp]->SetColor(GaugeUI::FrontColor, Color::blue);
	gauges_[(uint32_t)GaugeType::Stamina]->SetColor(GaugeUI::FrontColor, Color::green);

	bossHPGauge_->SetGaugePrame(mBoss->GetHpGaugeParam());
	bossHPGauge_->SetPos(Vec2(GetWindowHalfSize().x - 448.f, 880.f));
	bossHPGauge_->SetColor(GaugeUI::FrontColor, Color::red);
	bossHPGauge_->Init();

	negotiationUI_->Init();
	negotiationUI_->SetUIManager(this);

	messageUI_->Init();

	itemBoxmUiManager->Init();
	itemBoxmUiManager->SetPlayer(mPlayer);

	resultUI_->Init();
}

void UIManager::Update()
{
	// �f�o�b�O���̂ݎ��s
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

	if (mPlayer->GetisDissolve() == true)
	{
		resultUI_->SetisActive(true);
		resultUI_->SetResultType(ResultUI::ResultType::YouDiedStr);
	}
	else if (mBoss->GetisDissolve() == true)
	{
		resultUI_->SetisActive(true);
		resultUI_->SetResultType(ResultUI::ResultType::EnemyFelledStr);
	}

	// �A�C�e���{�b�N�X(�����̂��)
	itemBoxmUiManager->Update();

	// �v���C���[�̃Q�[�W
	for (uint32_t i = 0; i < gauges_.size(); i++)
	{
		gauges_[i]->SetGaugePrame(mPlayer->GetGaugeParam(i));
		gauges_[i]->Update();
	}

	bossHPGauge_->SetGaugePrame(mBoss->GetHpGaugeParam());
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

	itemBoxmUiManager->DrawFrontSprite();

	bossHPGauge_->DrawFrontSprite();

	resultUI_->DrawFrontSprite();
}

void UIManager::SetPlayer(Player* player)
{
	mPlayer = player;
}

void UIManager::SetBoss(Boss* boss)
{
	mBoss = boss;
}

NegotiationUI* UIManager::GetNegotiationUI()
{
	return negotiationUI_.get();
}

MessageUI* UIManager::GetMessageUI()
{
	return messageUI_.get();
}
