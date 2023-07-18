#include "ItemBoxUIManager.h"

ItemBoxUIManager::ItemBoxUIManager():
	itemUI_(std::make_unique<ItemUI>())
{
	for (uint32_t i = 0; i < itemBoxUIs_.size(); i++)
	{
		itemBoxUIs_[i] = std::make_unique<ItemBoxUI>();
	}
}

void ItemBoxUIManager::Init()
{
	// íÜêSç¿ïW
	parent.pos = Vec2(288, GetWindowSize().y - 256);
	parent.scale = 1.5f;
	parent.Update();

	for (uint32_t i = 0; i < itemBoxUIs_.size(); i++)
	{
		itemBoxUIs_[i]->Init();
	}
	itemUI_->Init();
	itemUI_->SetType(ItemType::HPBottle);
}

void ItemBoxUIManager::Update()
{
	float width = 92.f;
	float height = 56.f;
	itemBoxUIs_[0]->SetPos(Vec2(width, 0));	// ç∂
	itemBoxUIs_[1]->SetPos(Vec2(-width, 0));
	itemBoxUIs_[2]->SetPos(Vec2(0, height));
	itemBoxUIs_[3]->SetPos(Vec2(0, -height));
	for (uint32_t i = 0; i < itemBoxUIs_.size(); i++)
	{
		itemBoxUIs_[i]->Update(&parent);
	}
	itemUI_->SetPos(Vec2(0, height));
	itemUI_->Update(&parent);
}

void ItemBoxUIManager::DrawFrontSprite()
{
	for (uint32_t i = 0; i < itemBoxUIs_.size(); i++)
	{
		itemBoxUIs_[i]->DrawFrontSprite();
	}
	itemUI_->DrawFrontSprite();
}
