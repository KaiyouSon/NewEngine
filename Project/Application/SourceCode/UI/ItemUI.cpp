#include "ItemUI.h"

ItemUI::ItemUI() :
	mItemUI(std::make_unique<Sprite>())
{
}

void ItemUI::Init()
{
	mItemUI->scale = 0.325f;
}

void ItemUI::Update(Transform* parent)
{
	mItemUI->Update(parent);
}

void ItemUI::DrawFrontSprite()
{
	mItemUI->Draw();
}

void ItemUI::SettingsByType()
{
	switch (mItemType)
	{
		// HP�{�g��
	case ItemType::HPBottle:
	{
		mItemUI->SetTexture(TextureManager::GetTexture("BottleUI"));
		mItemUI->SetTextureRect(Vec2(0, 0), Vec2(256, 256));
		mItemUI->SetSize(256);
	}
	break;

	// MP�{�g��
	case ItemType::MPBottle:
	{
		mItemUI->SetTexture(TextureManager::GetTexture("BottleUI"));
		mItemUI->SetTextureRect(Vec2(256, 0), Vec2(512, 256));
		mItemUI->SetSize(256);
	}
	break;

	// MP�{�g��
	case ItemType::EmptyBottle:
	{
		mItemUI->SetTexture(TextureManager::GetTexture("BottleUI"));
		mItemUI->SetTextureRect(Vec2(512, 0), Vec2(768, 256));
		mItemUI->SetSize(256);
	}
	break;

	// MP�{�g��
	case ItemType::Club:
	{
		mItemUI->SetTexture(TextureManager::GetTexture("ClubUI"));
		mItemUI->SetTextureRect(Vec2(0, 0), Vec2(256, 256));
		mItemUI->SetSize(256);
	}
	break;

	default:
		break;

	}
}

void ItemUI::SetPos(const Vec2 pos)
{
	mItemUI->pos = pos;
}

void ItemUI::SetType(const ItemType itemType)
{
	mItemType = itemType;
	SettingsByType();
}

void ItemUI::SetAlpha(const float alpha)
{
	mItemUI->color.a = alpha;
}
