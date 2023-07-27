#pragma once
#include "GaugeUI.h"
#include "NegotiationUI.h"
#include "TutorialUI.h"
#include "ItemBoxUIManager.h"
#include "Player.h"
#include "Boss.h"

class UIManager
{
private:
	std::array<std::unique_ptr<GaugeUI>, 3> gauges_;

	std::unique_ptr<ItemBoxUIManager> itemBoxUIManager_;
	std::unique_ptr<GaugeUI> bossHPGauge_;
	std::unique_ptr<NegotiationUI> negotiationUI_;
	std::unique_ptr<TutorialUI> tutorialUI_;

private:
	Transform itemBoxUIParent;

	Player* player_;
	Boss* boss_;

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
	TutorialUI* GetTutorialUI();
};

