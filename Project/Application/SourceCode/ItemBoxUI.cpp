#include "ItemBoxUI.h"

ItemBoxUI::ItemBoxUI() :
	frame_(std::make_unique<Sprite>())
{
	//frame_->SetTexture(*TextureManager::)
}

void ItemBoxUI::Init()
{
	frame_->pos = 0;
	frame_->scale = 1;
}

void ItemBoxUI::Update()
{
}

void ItemBoxUI::DrawFrontSprite()
{
}
