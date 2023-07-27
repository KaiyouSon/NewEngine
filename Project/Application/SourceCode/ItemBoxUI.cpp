#include "ItemBoxUI.h"

ItemBoxUI::ItemBoxUI() :
	frame_(std::make_unique<Sprite>()),
	light_(std::make_unique<Sprite>()),
	isLightActive_(false)
{
	frame_->SetTexture(TextureManager::GetTexture("ItemBoxFrame"));
	frame_->SetSize(Vec2(144, 160));

	light_->SetTexture(TextureManager::GetTexture("ItemBoxLight"));
	light_->SetSize(Vec2(144, 160));

	light_->color = Color(0x897a51);
}

void ItemBoxUI::Init()
{
	frame_->pos = 0;
	frame_->scale = 1;

	light_->pos = 0;
	light_->scale = 1;
	light_->color.a = 0;
	lightActiveTimer_.SetLimitTimer(180);
}

void ItemBoxUI::Update(Transform* parent)
{
	if (isLightActive_ == true)
	{
		light_->color.a = sinf(lightActiveTimer_.GetTimeRate() * PI) * 255;

		lightActiveTimer_.Update(false, 30);
		if (lightActiveTimer_.GetisTimeOut() == true)
		{
			light_->color.a = 0;
			lightActiveTimer_.Reset();
			isLightActive_ = 0;
		}
	}

	frame_->Update(parent);
	light_->Update(parent);
}

void ItemBoxUI::DrawFrame()
{
	frame_->Draw();
}

void ItemBoxUI::DrawLight()
{
	light_->Draw();
}

void ItemBoxUI::SetPos(const Vec2 pos)
{
	frame_->pos = pos;
	light_->pos = pos;
}

void ItemBoxUI::SetScale(const Vec2 scale)
{
	frame_->scale = scale;
	light_->scale = scale;
}

void ItemBoxUI::SetFrameAlpha(const float alpha)
{
	frame_->color.a = alpha;
}

void ItemBoxUI::SetisLightActive(const bool isLightActive)
{
	isLightActive_ = isLightActive;
}

