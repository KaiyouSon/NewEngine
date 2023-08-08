#include "UIManager.h"

UIManager::UIManager() :
	mBossHPGauge(std::make_unique<GaugeUI>()),
	mNegotiationUI(std::make_unique<NegotiationUI>()),
	mMessageUI(std::make_unique<MessageUI>()),
	mItemBoxUiManager(std::make_unique<ItemBoxUIManager>()),
	mResultUI(std::make_unique<ResultUI>())
{
	for (uint32_t i = 0; i < mGauges.size(); i++)
	{
		mGauges[i] = std::make_unique<GaugeUI>();
	}
}

void UIManager::Init()
{
	for (uint32_t i = 0; i < mGauges.size(); i++)
	{
		mGauges[i]->SetGaugePrame(mPlayer->GetGaugeParam(i));
		mGauges[i]->SetPos(Vec2(144.f, (float)(48.f + i * 18.f)));
		mGauges[i]->Init();
	}

	mGauges[(uint32_t)GaugeType::Hp]->SetColor(GaugeUI::FrontColor, Color::red);
	mGauges[(uint32_t)GaugeType::Mp]->SetColor(GaugeUI::FrontColor, Color::blue);
	mGauges[(uint32_t)GaugeType::Stamina]->SetColor(GaugeUI::FrontColor, Color::green);

	mBossHPGauge->SetGaugePrame(mBoss->GetHpGaugeParam());
	mBossHPGauge->SetPos(Vec2(GetWindowHalfSize().x - 448.f, 880.f));
	mBossHPGauge->SetColor(GaugeUI::FrontColor, Color::red);
	mBossHPGauge->Init();

	mNegotiationUI->Init();
	mNegotiationUI->SetUIManager(this);

	mMessageUI->Init();

	mItemBoxUiManager->Init();
	mItemBoxUiManager->SetPlayer(mPlayer);

	mResultUI->Init();
}

void UIManager::Update()
{
	// デバッグ時のみ実行
	//ProcessAtDebugBulid([&]()
	//	{
	//		if (Key::GetKeyDown(DIK_1))
	//		{
	//			mResultUI->SetisActive(true);
	//			mResultUI->SetResultType(ResultUI::ResultType::EnemyFelledStr);
	//		}
	//		else if (Key::GetKeyDown(DIK_2))
	//		{
	//			mResultUI->SetisActive(true);
	//			mResultUI->SetResultType(ResultUI::ResultType::YouDiedStr);
	//		}
	//	});

	if (mPlayer->GetisDissolve() == true)
	{
		mResultUI->SetisActive(true);
		mResultUI->SetResultType(ResultUI::ResultType::YouDiedStr);
	}
	else if (mBoss->GetisDissolve() == true)
	{
		mResultUI->SetisActive(true);
		mResultUI->SetResultType(ResultUI::ResultType::EnemyFelledStr);
	}

	// アイテムボックス(左下のやつ)
	mItemBoxUiManager->Update();

	// プレイヤーのゲージ
	for (uint32_t i = 0; i < mGauges.size(); i++)
	{
		mGauges[i]->SetGaugePrame(mPlayer->GetGaugeParam(i));
		mGauges[i]->Update();
	}

	mBossHPGauge->SetGaugePrame(mBoss->GetHpGaugeParam());
	mBossHPGauge->Update();

	mNegotiationUI->Update();

	mMessageUI->Update();

	mResultUI->Update();
}

void UIManager::DrawFrontSprite()
{
	for (uint32_t i = 0; i < mGauges.size(); i++)
	{
		mGauges[i]->DrawFrontSprite();
	}

	mNegotiationUI->DrawFrontSprite();

	mMessageUI->DrawFrontSprite();

	mItemBoxUiManager->DrawFrontSprite();

	mBossHPGauge->DrawFrontSprite();

	mResultUI->DrawFrontSprite();
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
	return mNegotiationUI.get();
}

MessageUI* UIManager::GetMessageUI()
{
	return mMessageUI.get();
}
