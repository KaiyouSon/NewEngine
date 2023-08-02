#pragma once
#include "GaugeUI.h"
#include "NegotiationUI.h"
#include "MessageUI.h"
#include "ItemBoxUIManager.h"
#include "Player.h"
#include "Boss.h"
#include "ResultUI.h"

class UIManager
{
private:
	std::array<std::unique_ptr<GaugeUI>, 3> mGauges;

	std::unique_ptr<ItemBoxUIManager> mItemBoxUiManager;
	std::unique_ptr<GaugeUI> mBossHPGauge;
	std::unique_ptr<NegotiationUI> mNegotiationUI;
	std::unique_ptr<MessageUI> mMessageUI;
	std::unique_ptr<ResultUI> mResultUI;

private:
	Transform mItemBoxUIParent;

	Player* mPlayer;
	Boss* mBoss;

public:
	UIManager();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetPlayer(Player* player);
	void SetBoss(Boss* boss);

public:
	NegotiationUI* GetNegotiationUI();
	MessageUI* GetMessageUI();
};

