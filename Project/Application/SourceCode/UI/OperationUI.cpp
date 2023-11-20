#include "OperationUI.h"

OperationUI::OperationUI()
{
	for (uint32_t i = 0; i < OperationType::Size; i++)
	{
		mPair.push_back(std::make_unique<Pair>());
	}
}

void OperationUI::Init()
{
	for (uint32_t i = 0; i < mPair.size(); i++)
	{
		std::string tag = "Operation" + std::to_string(i + 1) + "Text";
		mPair[i]->mText->SetTexture(TextureManager::GetTexture(tag));

		float baseY = 348.f;
		float offsetY = (float)i * 64.f;
		mPair[i]->mBack->pos = Vec2(-10, baseY + offsetY);
		mPair[i]->mText->pos = Vec2(32, baseY + offsetY);
	}
}

void OperationUI::Update()
{
	for (uint32_t i = 0; i < mPair.size(); i++)
	{
		mPair[i]->mBack->Update();
		mPair[i]->mText->Update();
	}
}

void OperationUI::Draw()
{
	for (uint32_t i = 0; i < mPair.size(); i++)
	{
		mPair[i]->mBack->Draw("FrontSprite");
		mPair[i]->mText->Draw("FrontSprite");
	}
}

OperationUI::Pair::Pair() :
	mBack(std::make_unique<Sprite>()),
	mText(std::make_unique<Sprite>())
{
	mBack->SetTexture(TextureManager::GetTexture("OperationBack"));

	mBack->SetAnchorPoint(Vec2(0.0f, 0.5f));
	mText->SetAnchorPoint(Vec2(0.0f, 0.5f));

	mBack->scale.y = 2.f;
	mText->scale = 0.35f;

	mBack->color.a = 225.f;
	mText->color.a = 205.f;
}