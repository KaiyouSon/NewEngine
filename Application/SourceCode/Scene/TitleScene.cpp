#include "TitleScene.h"
#include "TextureManager.h"

void TitleScene::Init()
{
	TextureManager::LoadTexture("pic.png", "pic");
	Camera::current.pos = { 0,1,-15 };
	Camera::current.rot = { Radian(0),0,0 };

	obj.model = Model("AttackEnemy", true);
	obj.rot.y = Radian(180);
	//obj.texture = *TextureManager::GetTexture("pic");

	skyDome.model = Model("SkyDome");

	sprite.texture = renderTexture;
	sprite.anchorPoint = 0;
}

void TitleScene::Update()
{
	//obj.rot.y += Radian(1);
	const float speed = 10;

	sprite.pos.x += (Key::GetKey(DIK_RIGHT) - Key::GetKey(DIK_LEFT)) * speed;
	sprite.pos.y += (Key::GetKey(DIK_DOWN) - Key::GetKey(DIK_UP)) * speed;
	sprite.color.a += Key::GetKey(DIK_Q) - Key::GetKey(DIK_E);

	obj.pos.x += (Key::GetKey(DIK_D) - Key::GetKey(DIK_A));
	obj.pos.y += (Key::GetKey(DIK_W) - Key::GetKey(DIK_S));

	sprite.Update();
	skyDome.Update();
	obj.Update();
}

void TitleScene::DrawRenderTexture()
{
	renderTexture.PreDrawScene();
	skyDome.Draw();
	obj.Draw();
	renderTexture.PostDrawScene();
}

void TitleScene::DrawBackSprite()
{
	sprite.Draw();
}

void TitleScene::DrawModel()
{
	//obj.Draw();
}

void TitleScene::DrawFrontSprite()
{
}

void TitleScene::DrawDebugGui()
{
}
