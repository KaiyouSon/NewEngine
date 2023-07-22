#include "ItemBoxUIManager.h"

ItemBoxUIManager::ItemBoxUIManager()
{
	for (uint32_t i = 0; i < itemBoxUIs_.size(); i++)
	{
		itemBoxUIs_[i] = std::make_unique<ItemBoxUI>();
		itemUIs_[i] = std::make_unique<ItemUI>();
	}
}

void ItemBoxUIManager::Init()
{
	// íÜêSç¿ïW
	parent.pos = Vec2(288, GetWindowSize().y - 256);
	parent.scale = 1.25f;
	parent.Update();

	for (uint32_t i = 0; i < itemBoxUIs_.size(); i++)
	{
		itemBoxUIs_[i]->Init();
		itemUIs_[i]->Init();
	}
	itemUIs_[Position::Down]->SetType(ItemType::HPBottle);
	itemUIs_[Position::Right]->SetType(ItemType::Club);
}

void ItemBoxUIManager::Update()
{
	parent.pos = Vec2(224, GetWindowSize().y - 196);
	parent.Update();

	float width = 92.f;
	float height = 56.f;
	itemBoxUIs_[Position::Left]->SetPos(Vec2(-width, 0));
	itemBoxUIs_[Position::Right]->SetPos(Vec2(width, 0));
	itemBoxUIs_[Position::Up]->SetPos(Vec2(0, -height));
	itemBoxUIs_[Position::Down]->SetPos(Vec2(0, height));

	itemUIs_[Position::Right]->SetPos(Vec2(width, 0));
	itemUIs_[Position::Down]->SetPos(Vec2(0, height));

	for (uint32_t i = 0; i < itemBoxUIs_.size(); i++)
	{
		itemBoxUIs_[i]->SetAlpha(alpha_);
		itemUIs_[i]->SetAlpha(alpha_);

		itemBoxUIs_[i]->Update(&parent);
		itemUIs_[i]->Update(&parent);
	}

}

void ItemBoxUIManager::DrawFrontSprite()
{
	for (uint32_t i = 0; i < itemBoxUIs_.size(); i++)
	{
		itemBoxUIs_[i]->DrawFrontSprite();
		itemUIs_[i]->DrawFrontSprite();
	}
}

void ItemBoxUIManager::SetAlpha(const float alpha)
{
	alpha_ = alpha;
}
