#include "GaugeUI.h"

GaugeUI::GaugeUI()
{
	gaugeSize_.y = 14.f;
	for (uint32_t i = 0; i < sprites_.size(); i++)
	{
		sprites_[i] = std::make_unique<Sprite>();
		sprites_[i]->SetTexture(TextureManager::GetTexture("Gauge"));
		sprites_[i]->SetAnchorPoint(Vec2(0.f, 0.5f));
	}
	sprites_[BackFrame]->SetTexture(TextureManager::GetTexture("White"));
	sprites_[BackFrame]->color = Color(0.f, 0.f, 0.f, 155.f);
}

void GaugeUI::Init()
{
	// 描画範囲
	sprites_[FrontColor]->SetTextureRect(Vec2(160, 0), Vec2(191, 31));
	sprites_[CurrentPos]->SetTextureRect(Vec2(192, 0), Vec2(223, 31));
	sprites_[FrontFrame]->SetTextureRect(Vec2(64, 0), Vec2(95, 31));
	sprites_[FrontLeftFrame]->SetTextureRect(Vec2(0, 0), Vec2(63, 31));
	sprites_[FrontRightFrame]->SetTextureRect(Vec2(96, 0), Vec2(159, 31));
}

void GaugeUI::Update()
{
	CalcPosUpdate();
	CalcSizeUpdate();

	for (uint32_t i = 0; i < sprites_.size(); i++)
	{
		sprites_[i]->Update();
	}
}

void GaugeUI::DrawFrontSprite()
{
	sprites_[BackFrame]->Draw();
	//sprites_[BackColor]->Draw();
	sprites_[FrontColor]->Draw();
	sprites_[CurrentPos]->Draw();
	sprites_[FrontFrame]->Draw();
	sprites_[FrontLeftFrame]->Draw();
	sprites_[FrontRightFrame]->Draw();
}

void GaugeUI::CalcPosUpdate()
{
	// 可変
	for (uint32_t i = 0; i <= BackFrame; i++)
	{
		sprites_[i]->pos = basePos_;
	}
	sprites_[CurrentPos]->pos = basePos_ + Vec2(gaugeSize_.x, 0.5f);

	// 不変
	const float kOffset = -20.f;
	sprites_[BackFrame]->pos = basePos_ + Vec2(0, 1);
	sprites_[FrontFrame]->pos = basePos_ + Vec2(0, 1);
	sprites_[FrontLeftFrame]->pos = basePos_ + Vec2(kOffset, 0);
	sprites_[FrontRightFrame]->pos = basePos_ + Vec2(kOffset + constGaugeSize_.x, 0);
}

void GaugeUI::CalcSizeUpdate()
{
	// サイズ
	const Vec2 kLeftRightSize(32, 16);
	sprites_[BackFrame]->SetSize(constGaugeSize_ + Vec2(6, 0));
	sprites_[FrontFrame]->SetSize(constGaugeSize_);
	sprites_[FrontLeftFrame]->SetSize(kLeftRightSize);
	sprites_[FrontRightFrame]->SetSize(kLeftRightSize);
	sprites_[CurrentPos]->SetSize(Vec2(16, 16));
}

void GaugeUI::SetPos(const Vec2 pos)
{
	basePos_ = pos;

	// 可変
	for (uint32_t i = 0; i <= BackFrame; i++)
	{
		sprites_[i]->pos = basePos_;
	}
	sprites_[CurrentPos]->pos = basePos_ + Vec2(gaugeSize_.x, 0.5f);

	// 不変
	const float kOffset = -20.f;
	sprites_[FrontFrame]->pos = basePos_ + Vec2(0, 1);
	sprites_[FrontLeftFrame]->pos = basePos_ + Vec2(kOffset, 0);
	sprites_[FrontRightFrame]->pos = basePos_ + Vec2(kOffset + constGaugeSize_.x, 0);
}

void GaugeUI::SetColor(const uint32_t index, const Color color)
{
	sprites_[index]->color = color;

	if (index == FrontColor)
	{
		sprites_[CurrentPos]->color = color;
	}
}

void GaugeUI::SetGaugePrame(const GaugeParam gaugeParam)
{
	gaugeWidthMax_ = Min<float>(gaugeParam.max, 1080.f);

	constGaugeSize_ = Vec2(gaugeWidthMax_, gaugeSize_.y);

	// CurrentPosの画像のサイズは不変、座標を可変
	gaugeSize_.x = gaugeParam.rate * gaugeWidthMax_;
	sprites_[FrontColor]->SetSize(gaugeSize_);
	sprites_[BackColor]->SetSize(gaugeSize_);
}
