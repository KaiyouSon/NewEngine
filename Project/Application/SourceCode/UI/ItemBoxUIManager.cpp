#include "ItemBoxUIManager.h"

ItemBoxUIManager::ItemBoxUIManager() :
	mNumber(std::make_unique<Sprite>())
{
	for (uint32_t i = 0; i < mItemBoxUIs.size(); i++)
	{
		mItemBoxUIs[i] = std::make_unique<ItemBoxUI>();
		mItemUIs[i] = std::make_unique<ItemUI>();
	}

	mNumber->SetTexture(TextureManager::GetTexture("NumberSheets"));
	mNumber->SetSize(96);
}

void ItemBoxUIManager::Init()
{
	// 中心座標
	mParent.pos = Vec2(288, GetWindowSize().y - 256);
	mParent.scale = 1.25f;
	mParent.Update();

	for (uint32_t i = 0; i < mItemBoxUIs.size(); i++)
	{
		mItemBoxUIs[i]->Init();
		mItemUIs[i]->Init();
	}
	mItemUIs[Position::Down]->SetType(ItemType::HPBottle);
	mItemUIs[Position::Right]->SetType(ItemType::Club);

	mNotActiveTimer.SetLimitTimer(180);
}

void ItemBoxUIManager::Update()
{
	mNumber->scale = 0.4f;
	mNumber->pos = Vec2(28, 86);

	bool isLeftStickMove = Pad::GetStick(PadCode::LeftStick, 300) != 0;
	bool isAnyButtonDown = Pad::GetAnyButtonDown();

	if (isLeftStickMove || isAnyButtonDown)
	{
		mNotActiveTimer.Reset();
		mIsActive = true;
	}
	else
	{
		mNotActiveTimer.Update();
		if (mNotActiveTimer == true)
		{
			mIsActive = false;
		}
	}

	float speed = 15;
	if (mIsActive == true)
	{
		mAlpha += speed;
	}
	else
	{
		mAlpha -= speed;
	}
	mAlpha = Clamp<float>(mAlpha, 0, 255);

	if (mIsActive == true)
	{
		if (Pad::GetButtonDown(PadCode::ButtonRight))
		{
			mItemBoxUIs[Position::Right]->SetisLightActive(true);
		}
		else if (Pad::GetButtonDown(PadCode::ButtonLeft))
		{
			mItemBoxUIs[Position::Left]->SetisLightActive(true);
		}
		else if (Pad::GetButtonDown(PadCode::ButtonUp))
		{
			mItemBoxUIs[Position::Up]->SetisLightActive(true);
		}
		else if (Pad::GetButtonDown(PadCode::ButtonDown))
		{
			mItemBoxUIs[Position::Down]->SetisLightActive(true);
		}
	}

	mParent.pos = Vec2(224, GetWindowSize().y - 196);
	mParent.Update();

	float width = 92.f;
	float height = 56.f;
	mItemBoxUIs[Position::Left]->SetPos(Vec2(-width, 0));
	mItemBoxUIs[Position::Right]->SetPos(Vec2(width, 0));
	mItemBoxUIs[Position::Up]->SetPos(Vec2(0, -height));
	mItemBoxUIs[Position::Down]->SetPos(Vec2(0, height));

	mItemUIs[Position::Right]->SetPos(Vec2(width, 0));
	mItemUIs[Position::Down]->SetPos(Vec2(0, height));

	if (mPlayer->GetBottleNum() <= 0)
	{
		mItemUIs[Position::Down]->SetType(ItemType::EmptyBottle);
	}

	for (uint32_t i = 0; i < mItemBoxUIs.size(); i++)
	{
		mItemBoxUIs[i]->SetFrameAlpha(mAlpha);
		mItemUIs[i]->SetAlpha(mAlpha);

		mItemBoxUIs[i]->Update(&mParent);
		mItemUIs[i]->Update(&mParent);
	}

	mNumber->color.a = mAlpha;
	Vec2 leftUp = { 0 + (float)mPlayer->GetBottleNum() * 96,0 };
	Vec2 rightDown = { 96 + (float)mPlayer->GetBottleNum() * 96,96 };
	mNumber->SetTextureRect(leftUp, rightDown);

	mNumber->Update(&mParent);
}

void ItemBoxUIManager::DrawFrontSprite()
{
	for (uint32_t i = 0; i < mItemBoxUIs.size(); i++)
	{
		mItemBoxUIs[i]->DrawFrame();
		mItemUIs[i]->DrawFrontSprite();
		mItemBoxUIs[i]->DrawLight();
	}

	mNumber->Draw();
}

void ItemBoxUIManager::SetAlpha(const float alpha)
{
	mAlpha = alpha;
}

void ItemBoxUIManager::SetPlayer(Player* player)
{
	mPlayer = player;
}
