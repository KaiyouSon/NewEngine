#include "GameScene.h"
#include "TitleScene.h"

GameScene::GameScene()
{
}
GameScene::~GameScene()
{
}

void GameScene::Init()
{
	Camera::current.pos = { 0,15,-15 };
	Camera::current.rot = { Radian(45),0,0 };
	Light::current.Init();
	Light::current.lightDir = { 0,1,5 };

	Model model = Model("sphere", true);
	Texture tex = Texture("pic.png");

	obj.model = model;
}

void GameScene::Update()
{
	obj.Update();

	if (Key::GetKey(DIK_RIGHT))
	{
		obj.rot.y -= Radian(5);
	}
	if (Key::GetKey(DIK_LEFT))
	{
		obj.rot.y += Radian(5);
	}

	if (Key::GetKey(DIK_UP))
	{
		obj.color.r++;
	}
	if (Key::GetKey(DIK_DOWN))
	{
		obj.color.r--;
	}

	Camera::DebugCameraUpdate();

	Light::current.Update();
}

void GameScene::DrawBackSprite()
{
}

void GameScene::DrawModel()
{
	obj.Draw();
}

void GameScene::DrawFrontSprite()
{
}

void GameScene::DrawDebugGui()
{
	//GuiManager::BeginWindow();

	//GuiManager::EndWindow();
}
