#include "PlayerGaugeUI.h"

PlayerGaugeUI::PlayerGaugeUI()
{
	for (uint32_t i = 0; i < mGauges.size(); i++)
	{
		mGauges[i] = std::make_unique<GaugeUI>();
		mGaugeParam[i] = GaugeParam();
	}
}

void PlayerGaugeUI::Init()
{
	// ゲージの初期化
	for (uint32_t i = 0; i < mGauges.size(); i++)
	{
		mGauges[i]->SetPos(Vec2(144.f, (float)(48.f + i * 18.f)));
		mGauges[i]->Init();
	}

	// ゲージの種類に色を設定
	mGauges[(uint32_t)GaugeType::Hp]->SetColor(GaugeUI::FrontColor, Color::red);
	mGauges[(uint32_t)GaugeType::Mp]->SetColor(GaugeUI::FrontColor, Color::blue);
	mGauges[(uint32_t)GaugeType::Stamina]->SetColor(GaugeUI::FrontColor, Color::green);
}

void PlayerGaugeUI::Update()
{
	// プレイヤーのゲージ
	for (uint32_t i = 0; i < mGauges.size(); i++)
	{
		mGauges[i]->SetGaugePrame(mGaugeParam[i]);
		mGauges[i]->Update();
	}
}

void PlayerGaugeUI::Draw()
{
	for (uint32_t i = 0; i < mGauges.size(); i++)
	{
		mGauges[i]->Draw();
	}
}

// セッター
void PlayerGaugeUI::SetGaugeParam(const GaugeParam gaugeParam, const uint32_t index)
{
	mGaugeParam[index] = gaugeParam;
}
