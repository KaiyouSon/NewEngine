#include "ItemBoxUI.h"

ItemBoxUI::ItemBoxUI() :
	frame_(std::make_unique<Sprite>())
{
	frame_->SetTexture(TextureManager::GetTexture("ItemBoxFrame"));
	frame_->SetSize(Vec2(144, 160));
}

void ItemBoxUI::Init()
{
	frame_->pos = 0;
	frame_->scale = 1;
}

void ItemBoxUI::Update(Transform* parent)
{
	frame_->Update(parent);
}

void ItemBoxUI::DrawFrontSprite()
{
	frame_->Draw();
}

void ItemBoxUI::SetPos(const Vec2 pos)
{
	frame_->pos = pos;
}

void ItemBoxUI::SetScale(const Vec2 scale)
{
	frame_->scale = scale;
}
