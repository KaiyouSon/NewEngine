#include "UIManager.h"
#include "ItemBoxUIGroup.h"
#include "PlayerGaugeUI.h"
#include "BossGaugeUI.h"
#include "ResultUI.h"
#include "OperationUI.h"

UIManager::UIManager()
{
	// 交渉UI
	mUI.push_back(std::move(std::make_unique<NegotiationUI>()));

	// メッセージUI
	mUI.push_back(std::move(std::make_unique<MessageUI>()));

	// メッセージUI
	mUI.push_back(std::move(std::make_unique<ResultUI>()));

	// アイテムボックスのUIのグループ
	mUI.push_back(std::move(std::make_unique<ItemBoxUIGroup>()));

	// プレイヤーのゲージUI
	mUI.push_back(std::move(std::make_unique<PlayerGaugeUI>()));

	// ボスのゲージUI
	mUI.push_back(std::move(std::make_unique<BossGaugeUI>()));

	// 操作UI
	mUI.push_back(std::move(std::make_unique<OperationUI>()));
}

void UIManager::Init()
{
	for (uint32_t i = 0; i < mUI.size(); i++)
	{
		mUI[i]->Init();
	}
}
void UIManager::Update()
{
	if (mMovieEvent->GetisPlaying() == true)
	{
		return;
	}

	// 交渉UIの処理
	NegotiationUIUpdate();

	// リザルトUIの処理
	ResultUIUpdate();

	// アイテムボックスUIのグループの処理
	ItemBoxUIGroupUpdate();

	// プレイヤーのゲージのUI
	PlayerGaugeUIUpdate();

	// ボスのゲージUI
	BossGaugeUIUpdate();

	for (uint32_t i = 0; i < mUI.size(); i++)
	{
		mUI[i]->Update();
	}
}
void UIManager::DrawFrontSprite()
{
	if (mMovieEvent->GetisPlaying() == true)
	{
		return;
	}

	for (uint32_t i = 0; i < mUI.size(); i++)
	{
		mUI[i]->Draw();
	}
}

// 交渉UIの処理
void UIManager::NegotiationUIUpdate()
{
	NegotiationUI* negotiationUI = GetNegotiationUI();
	if (negotiationUI->GetisActive() == false)
	{
		return;
	}

	// 種類別の処理
	switch (negotiationUI->GetTextType())
	{
	case NegotiationUI::TextType::ReadMessageText:
		if (Pad::GetButtonDown(PadCode::ButtonB))
		{
			MessageUI* messageUI = GetMessageUI();
			if (messageUI->GetisActive() == true)
			{
				messageUI->SetisActive(false);
			}
			else
			{
				messageUI->SetisActive(true);
			}
		}
		break;
	}

}

// リザルトUIの処理
void UIManager::ResultUIUpdate()
{
	// インスタンス取得
	ResultUI* resultUI =
		dynamic_cast<ResultUI*>(mUI[(uint32_t)UIIndex::ResultUI].get());

	if (mPlayer->GetisDissolve() == true)
	{
		resultUI->SetisActive(true);
		resultUI->SetResultType(ResultUI::ResultType::YouDiedText);
	}
	else if (mBoss->GetisDissolve() == true)
	{
		resultUI->SetisActive(true);
		resultUI->SetResultType(ResultUI::ResultType::EnemyFelledText);
	}
}

// アイテムボックスUIのグループの処理
void UIManager::ItemBoxUIGroupUpdate()
{
	// インスタンス取得
	ItemBoxUIGroup* itemBoxUIGroup =
		dynamic_cast<ItemBoxUIGroup*>(mUI[(uint32_t)UIIndex::ItemBoxUIGroup].get());

	itemBoxUIGroup->SetBottleNum(mPlayer->GetBottleNum());
}

// プレイヤーのゲージのUI
void UIManager::PlayerGaugeUIUpdate()
{
	// インスタンス取得
	PlayerGaugeUI* playerGaugeUI =
		dynamic_cast<PlayerGaugeUI*>(mUI[(uint32_t)UIIndex::PlayerGaugeUI].get());

	for (uint32_t i = 0; i < 3; i++)
	{
		playerGaugeUI->SetGaugeParam(mPlayer->GetGaugeParam(i), i);
	}
}

// ボスのゲージUI
void UIManager::BossGaugeUIUpdate()
{
	// インスタンス取得
	BossGaugeUI* bossGaugeUI =
		dynamic_cast<BossGaugeUI*>(mUI[(uint32_t)UIIndex::BossGaugeUI].get());

	bossGaugeUI->SetisActive(mBoss->GetisFight());

	bossGaugeUI->SetGaugeParam(mBoss->GetHpGaugeParam());
}


// セッター
void UIManager::SetPlayer(Player* player)
{
	mPlayer = player;
}
void UIManager::SetBoss(Boss* boss)
{
	mBoss = boss;
}
void UIManager::SetMovieEvent(MovieEvent* movieEvent)
{
	mMovieEvent = movieEvent;
}

// ゲッター
NegotiationUI* UIManager::GetNegotiationUI()
{
	return dynamic_cast<NegotiationUI*>(mUI[(uint32_t)UIIndex::NegotiationUI].get());
}
MessageUI* UIManager::GetMessageUI()
{
	return dynamic_cast<MessageUI*>(mUI[(uint32_t)UIIndex::MessageUI].get());
}
