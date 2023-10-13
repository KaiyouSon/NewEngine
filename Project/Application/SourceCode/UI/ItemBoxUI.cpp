#include "ItemBoxUI.h"

ItemBoxUI::ItemBoxUI() :
	mFrame(std::make_unique<Sprite>()),
	mLight(std::make_unique<Sprite>()),
	mIsLightActive(false)
{
	mFrame->SetTexture(TextureManager::GetTexture("ItemBoxFrame"));
	mFrame->SetSize(Vec2(144, 160));

	mLight->SetTexture(TextureManager::GetTexture("ItemBoxLight"));
	mLight->SetSize(Vec2(144, 160));

	mLight->color = Color(0x897a51);
}

void ItemBoxUI::Init()
{
	mFrame->pos = 0;
	mFrame->scale = 1;

	mLight->pos = 0;
	mLight->scale = 1;
	mLight->color.a = 0;
	mLightActiveTimer.SetLimitTimer(180);
}

void ItemBoxUI::Update(Transform* parent)
{
	if (mIsLightActive == true)
	{
		mLight->color.a = sinf(mLightActiveTimer.GetTimeRate() * PI) * 255;

		mLightActiveTimer.Update(30);
		if (mLightActiveTimer == true)
		{
			mLight->color.a = 0;
			mLightActiveTimer.Reset();
			mIsLightActive = 0;
		}
	}

	mFrame->Update(parent);
	mLight->Update(parent);
}

void ItemBoxUI::DrawFrame()
{
	mFrame->Draw();
}

void ItemBoxUI::DrawLight()
{
	mLight->Draw();
}

void ItemBoxUI::SetPos(const Vec2 pos)
{
	mFrame->pos = pos;
	mLight->pos = pos;
}

void ItemBoxUI::SetScale(const Vec2 scale)
{
	mFrame->scale = scale;
	mLight->scale = scale;
}

void ItemBoxUI::SetFrameAlpha(const float alpha)
{
	mFrame->color.a = alpha;
}

void ItemBoxUI::SetisLightActive(const bool isLightActive)
{
	mIsLightActive = isLightActive;
}

