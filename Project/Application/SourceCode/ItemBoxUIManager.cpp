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

	notActiveTimer_.SetLimitTimer(180);
}

void ItemBoxUIManager::Update()
{
	bool isLeftStickMove = Pad::GetStick(PadCode::LeftStick, 300) != 0;
	bool isAnyButtonDown = Pad::GetAnyButtonDown();

	if (isLeftStickMove || isAnyButtonDown)
	{
		notActiveTimer_.Reset();
		isActive_ = true;
	}
	else
	{
		notActiveTimer_.Update(false);
		if (notActiveTimer_.GetisTimeOut() == true)
		{
			isActive_ = false;
		}
	}

	float speed = 15;
	if (isActive_ == true)
	{
		alpha_ += speed;
	}
	else
	{
		alpha_ -= speed;
	}
	alpha_ = Clamp<float>(alpha_, 0, 255);

	if (isActive_ == true)
	{
		if (Pad::GetButtonDown(PadCode::ButtonRight))
		{
			itemBoxUIs_[Position::Right]->SetisLightActive(true);
		}
		else if (Pad::GetButtonDown(PadCode::ButtonLeft))
		{
			itemBoxUIs_[Position::Left]->SetisLightActive(true);
		}
		else if (Pad::GetButtonDown(PadCode::ButtonUp))
		{
			itemBoxUIs_[Position::Up]->SetisLightActive(true);
		}
		else if (Pad::GetButtonDown(PadCode::ButtonDown))
		{
			itemBoxUIs_[Position::Down]->SetisLightActive(true);
		}
	}

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
		itemBoxUIs_[i]->SetFrameAlpha(alpha_);
		//itemBoxUIs_[i]->SetLightAlpha(lightAlpha_);
		itemUIs_[i]->SetAlpha(alpha_);

		itemBoxUIs_[i]->Update(&parent);
		itemUIs_[i]->Update(&parent);
	}
}

void ItemBoxUIManager::DrawFrontSprite()
{
	for (uint32_t i = 0; i < itemBoxUIs_.size(); i++)
	{
		itemBoxUIs_[i]->DrawFrame();
		itemUIs_[i]->DrawFrontSprite();
		itemBoxUIs_[i]->DrawLight();
	}
}

void ItemBoxUIManager::SetAlpha(const float alpha)
{
	alpha_ = alpha;
}
