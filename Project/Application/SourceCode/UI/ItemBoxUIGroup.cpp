#include "ItemBoxUIGroup.h"

ItemBoxUIGroup::ItemBoxUIGroup() :
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

void ItemBoxUIGroup::Init()
{
	// 初期化
	for (uint32_t i = 0; i < mItemBoxUIs.size(); i++)
	{
		mItemBoxUIs[i]->Init();
		mItemUIs[i]->Init();
	}
	mItemUIs[Position::Down]->SetType(ItemType::HPBottle);
	mItemUIs[Position::Right]->SetType(ItemType::Club);

	mNotActiveTimer.SetLimitTimer(180);
	mNotActiveTimer.Reset();

	// 親行列を基の座標初期化
	mParent.pos = Vec2(224, GetWindowSize().y - 196);
	mParent.scale = 1.25f;
	mParent.Update();

	float width = 92.f;
	float height = 56.f;
	mItemBoxUIs[Position::Left]->SetPos(Vec2(-width, 0));
	mItemBoxUIs[Position::Right]->SetPos(Vec2(width, 0));
	mItemBoxUIs[Position::Up]->SetPos(Vec2(0, -height));
	mItemBoxUIs[Position::Down]->SetPos(Vec2(0, height));

	mItemUIs[Position::Right]->SetPos(Vec2(width, 0));
	mItemUIs[Position::Down]->SetPos(Vec2(0, height));

	// 数字の初期化
	mNumber->scale = 0.4f;
	mNumber->pos = Vec2(28, 86);
}

void ItemBoxUIGroup::Update()
{
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

	// スプライトのαの処理
	AlphaUpdate();

	// ボタンを押したときに光る処理
	ShainUpdate();

	// アイテムボトルの処理
	ItemBottleUpdate();

	for (uint32_t i = 0; i < mItemBoxUIs.size(); i++)
	{
		// α設定
		mItemBoxUIs[i]->SetFrameAlpha(mAlpha);
		mItemUIs[i]->SetAlpha(mAlpha);

		// 更新処理
		mItemBoxUIs[i]->Update(&mParent);
		mItemUIs[i]->Update(&mParent);
	}
	mNumber->Update(&mParent);
}

void ItemBoxUIGroup::Draw()
{
	bool isDisable = mIsActive == false && mAlpha == 0;	// 描画するかどうか
	if (isDisable == true)
	{
		return;
	}

	for (uint32_t i = 0; i < mItemBoxUIs.size(); i++)
	{
		mItemBoxUIs[i]->DrawFrame();
		mItemUIs[i]->DrawFrontSprite();
		mItemBoxUIs[i]->DrawLight();
	}

	mNumber->Draw();
}

// スプライトのαの処理
void ItemBoxUIGroup::AlphaUpdate()
{
	float speed = 15;
	mAlpha += (mIsActive == true) ? speed : -speed;
	mAlpha = Clamp<float>(mAlpha, 0, 255);
}

// ボタンを押したときに光る処理
void ItemBoxUIGroup::ShainUpdate()
{
	if (mIsActive == false)
	{
		return;
	}

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

// アイテムボトルの処理
void ItemBoxUIGroup::ItemBottleUpdate()
{
	if (mBottleNum <= 0)
	{
		mItemUIs[Position::Down]->SetType(ItemType::EmptyBottle);
	}

	mNumber->color.a = mAlpha;

	// テクスチャの処理
	Vec2 leftUp = { 0 + (float)mBottleNum * 96,0 };
	Vec2 rightDown = { 96 + (float)mBottleNum * 96,96 };
	mNumber->SetTextureRect(leftUp, rightDown);
}

void ItemBoxUIGroup::SetAlpha(const float alpha)
{
	mAlpha = alpha;
}

void ItemBoxUIGroup::SetBottleNum(const uint32_t bottleNum)
{
	mBottleNum = bottleNum;
}
