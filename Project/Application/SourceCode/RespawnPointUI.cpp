#include "RespawnPointUI.h"

RespawnPointUI::RespawnPointUI() :
	mBack(std::make_unique<Sprite>())
{
	mBack->SetTexture(TextureManager::GetTexture("RespawnBack"));
	Init();
}

void RespawnPointUI::Init()
{
	mBack->pos.x = -4;
	mBack->scale = 2;
	mBack->SetAnchorPoint(Vec2::zero);
}

void RespawnPointUI::Update()
{
	mBack->Update();
}

void RespawnPointUI::DrawFrontSprite()
{
	//mBack->Draw();
}
