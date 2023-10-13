#include "BossGaugeUI.h"

BossGaugeUI::BossGaugeUI() :
	mGauge(std::make_unique<GaugeUI>()), mAlpha(0)
{
}

void BossGaugeUI::Init()
{
	mGauge->SetPos(Vec2(GetWindowHalfSize().x - 448.f, 880.f));
	mGauge->SetColor(GaugeUI::FrontColor, Color::red);
	mGauge->Init();

	mIsActive = false;
}

void BossGaugeUI::Update()
{
	AlphaUpdate();
	mGauge->Update();
}

void BossGaugeUI::Draw()
{
	bool isDisable = mIsActive == false && mAlpha == 0;	// 描画するかどうか
	if (isDisable == true)
	{
		return;
	}

	mGauge->Draw();
}

void BossGaugeUI::AlphaUpdate()
{
	float speed = 35;
	mAlpha += (mIsActive == true) ? speed : -speed;
	mAlpha = Clamp<float>(mAlpha, 0.f, 255.f);

	mGauge->SetAlpha(mAlpha);
}

void BossGaugeUI::SetGaugeParam(const GaugeParam gaugeParam)
{
	mGauge->SetGaugePrame(gaugeParam);
}
