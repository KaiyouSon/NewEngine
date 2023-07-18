#include "ItemUI.h"

ItemUI::ItemUI() :
	itemUI_(std::make_unique<Sprite>())
{
}

void ItemUI::Init()
{
	itemUI_->scale = 0.35f;
}

void ItemUI::Update(Transform* parent)
{
	itemUI_->Update(parent);
}

void ItemUI::DrawFrontSprite()
{
	itemUI_->Draw();
}

void ItemUI::SettingsByType()
{
	switch (itemType_)
	{

		// HPボトル
	case ItemType::HPBottle:
	{
		itemUI_->SetTexture(TextureManager::GetTexture("BottleUI"));
		itemUI_->SetTextureRect(Vec2(0, 0), Vec2(256, 256));
		itemUI_->SetSize(256);
	}
	break;

	// MPボトル
	case ItemType::MPBottle:
	{
		itemUI_->SetTexture(TextureManager::GetTexture("BottleUI"));
		itemUI_->SetTextureRect(Vec2(256, 0), Vec2(512, 256));
		itemUI_->SetSize(256);
	}
	break;

	default:
		break;

	}
}

void ItemUI::SetPos(const Vec2 pos)
{
	itemUI_->pos = pos;
}

void ItemUI::SetType(const ItemType itemType)
{
	itemType_ = itemType;
	SettingsByType();
}