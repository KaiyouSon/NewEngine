#include "TitleScene.h"
#include "TextureManager.h"

void TitleScene::Init()
{
	TextureManager::LoadTexture("pic.png", "pic");

	obj.model = Model("Sphere", true);
	obj.pos.z = 5.f;
	obj.texture = *TextureManager::GetTexture("pic");

	spr.pos = GetWindowHalfSize();
	spr.texture = *TextureManager::GetTexture("pic");
	spr.endRadian = Radian(45);
}

void TitleScene::Update()
{
	if (Key::GetKey(DIK_Q))
	{
		spr.endRadian += Radian(1);
	}
	if (Key::GetKey(DIK_E))
	{
		spr.endRadian -= Radian(1);
	}

	if (Key::GetKey(DIK_A))
	{
		spr.startRadian += Radian(1);
	}
	if (Key::GetKey(DIK_D))
	{
		spr.startRadian -= Radian(1);
	}


	obj.Update();
	spr.Update();
}

void TitleScene::DrawRenderTexture()
{
}

void TitleScene::DrawBackSprite()
{
}

void TitleScene::DrawModel()
{
	//obj.Draw();
}

void TitleScene::DrawFrontSprite()
{
	spr.Draw();
}

void TitleScene::DrawDebugGui()
{
}
