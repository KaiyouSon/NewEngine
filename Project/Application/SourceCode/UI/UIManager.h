#pragma once
#include "GaugeUI.h"
#include "NegotiationUI.h"
#include "MessageUI.h"
#include "Player.h"
#include "Boss.h"
#include "MovieEvent.h"

// 全UIを管理するクラス
class UIManager
{
private:
	enum class UIIndex
	{
		NegotiationUI,
		MessageUI,
		ResultUI,
		ItemBoxUIGroup,
		PlayerGaugeUI,
		BossGaugeUI
	};

private:
	std::vector<std::unique_ptr<IUI>> mUI;

private:
	Transform mItemBoxUIParent;

	Player* mPlayer;
	Boss* mBoss;
	MovieEvent* mMovieEvent;
	bool mIsActive;

private:
	// 交渉UIの処理
	void NegotiationUIUpdate();

	// リザルトUIの処理
	void ResultUIUpdate();

	// アイテムボックスUIのグループの処理
	void ItemBoxUIGroupUpdate();

	// プレイヤーのゲージのUI
	void PlayerGaugeUIUpdate();

	// ボスのゲージUI
	void BossGaugeUIUpdate();

public:
	UIManager();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetPlayer(Player* player);
	void SetBoss(Boss* boss);
	void SetMovieEvent(MovieEvent* movieEvent);

public:
	NegotiationUI* GetNegotiationUI();
	MessageUI* GetMessageUI();
};

