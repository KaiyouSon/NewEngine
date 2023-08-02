#include "GaugeUI.h"

GaugeUI::GaugeUI()
{
	mGaugeSize.y = 14.f;
	for (uint32_t i = 0; i < mSprites.size(); i++)
	{
		mSprites[i] = std::make_unique<Sprite>();
		mSprites[i]->SetTexture(TextureManager::GetTexture("Gauge"));
		mSprites[i]->SetAnchorPoint(Vec2(0.f, 0.5f));
	}
	mSprites[BackFrame]->SetTexture(TextureManager::GetTexture("White"));
	mSprites[BackFrame]->color = Color(0.f, 0.f, 0.f, 155.f);

	mSprites[BackColor]->SetTexture(TextureManager::GetTexture("White"));
	mSprites[BackColor]->color = Color(0x776116);

	mStayTimer.SetLimitTimer(60);
}

void GaugeUI::Init()
{
	// 描画範囲
	mSprites[FrontColor]->SetTextureRect(Vec2(160, 0), Vec2(191, 31));
	mSprites[CurrentPos]->SetTextureRect(Vec2(192, 0), Vec2(223, 31));
	mSprites[FrontFrame]->SetTextureRect(Vec2(64, 0), Vec2(95, 31));
	mSprites[FrontLeftFrame]->SetTextureRect(Vec2(0, 0), Vec2(63, 31));
	mSprites[FrontRightFrame]->SetTextureRect(Vec2(96, 0), Vec2(159, 31));

	mBackGaugeSize = mGaugeSize;
}

void GaugeUI::Update()
{
	CalcPosUpdate();
	CalcSizeUpdate();

	if (mBackGaugeSize.x > mGaugeSize.x)
	{
		mStayTimer.Update();
		if (mStayTimer == true)
		{
			mBackGaugeSize.x -= 5.f;
		}
	}
	else
	{
		mStayTimer.Reset();
	}
	mBackGaugeSize.x = Max<float>(mGaugeSize.x, mBackGaugeSize.x);
	mSprites[BackColor]->SetSize(mBackGaugeSize);

	for (uint32_t i = 0; i < mSprites.size(); i++)
	{
		mSprites[i]->Update();
	}
}

void GaugeUI::DrawFrontSprite()
{
	mSprites[BackFrame]->Draw();
	mSprites[BackColor]->Draw();
	mSprites[FrontColor]->Draw();
	mSprites[CurrentPos]->Draw();
	mSprites[FrontFrame]->Draw();
	mSprites[FrontLeftFrame]->Draw();
	mSprites[FrontRightFrame]->Draw();
}

void GaugeUI::CalcPosUpdate()
{
	// 可変
	for (uint32_t i = 0; i <= BackFrame; i++)
	{
		mSprites[i]->pos = mBasePos;
	}
	mSprites[CurrentPos]->pos = mBasePos + Vec2(mGaugeSize.x, 0.5f);

	// 不変
	const float kOffset = -20.f;
	mSprites[BackFrame]->pos = mBasePos + Vec2(0, 1);
	mSprites[FrontFrame]->pos = mBasePos + Vec2(0, 1);
	mSprites[FrontLeftFrame]->pos = mBasePos + Vec2(kOffset, 0);
	mSprites[FrontRightFrame]->pos = mBasePos + Vec2(kOffset + mConstGaugeSize.x, 0);
}

void GaugeUI::CalcSizeUpdate()
{
	// サイズ
	const Vec2 kLeftRightSize(32, 16);
	mSprites[BackFrame]->SetSize(mConstGaugeSize + Vec2(6, 0));
	mSprites[FrontFrame]->SetSize(mConstGaugeSize);
	mSprites[FrontLeftFrame]->SetSize(kLeftRightSize);
	mSprites[FrontRightFrame]->SetSize(kLeftRightSize);
	mSprites[CurrentPos]->SetSize(Vec2(16, 16));
}

void GaugeUI::SetPos(const Vec2 pos)
{
	mBasePos = pos;

	// 可変
	for (uint32_t i = 0; i <= BackFrame; i++)
	{
		mSprites[i]->pos = mBasePos;
	}
	mSprites[CurrentPos]->pos = mBasePos + Vec2(mGaugeSize.x, 0.5f);

	// 不変
	const float kOffset = -20.f;
	mSprites[FrontFrame]->pos = mBasePos + Vec2(0, 1);
	mSprites[FrontLeftFrame]->pos = mBasePos + Vec2(kOffset, 0);
	mSprites[FrontRightFrame]->pos = mBasePos + Vec2(kOffset + mConstGaugeSize.x, 0);
}

void GaugeUI::SetColor(const uint32_t index, const Color color)
{
	mSprites[index]->color = color;

	if (index == FrontColor)
	{
		mSprites[CurrentPos]->color = color;
	}
}

void GaugeUI::SetGaugePrame(const GaugeParam gaugeParam)
{
	mGaugeWidthMax = Min<float>(gaugeParam.max, 896.f);

	mConstGaugeSize = Vec2(mGaugeWidthMax, mGaugeSize.y);

	// CurrentPosの画像のサイズは不変、座標を可変
	mGaugeSize.x = gaugeParam.rate * mGaugeWidthMax;
	mSprites[FrontColor]->SetSize(mGaugeSize);
}
