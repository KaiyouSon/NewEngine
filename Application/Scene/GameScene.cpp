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
	Light::current.lightPos = { 1,1,1 };

	Texture tex = Texture("pic.png");
	obj.model = Model("sphere", true);
	obj2.model = Model("sphere");

	obj.pos.x = 3;
	obj2.pos.x = -3;

	obj.isLighting = true;
	obj2.isLighting = true;

	spr.texture = tex;
	spr.scale = 0.25f;
	spr.anchorPoint = 0.5f;
}

void GameScene::Update()
{
	obj.Update();
	obj2.Update();

	obj.rot.y -= Radian(3);
	obj2.rot.y -= Radian(3);

	if (Key::GetKey(DIK_UP))
	{
		Light::current.lightPos.z += 0.1f;
	}
	if (Key::GetKey(DIK_DOWN))
	{
		Light::current.lightPos.z -= 0.1f;
	}
	if (Key::GetKey(DIK_RIGHT))
	{
		Light::current.lightPos.x += 0.1f;
	}
	if (Key::GetKey(DIK_LEFT))
	{
		Light::current.lightPos.x -= 0.1f;
	}

	spr.Update();

	Camera::DebugCameraUpdate();
}

void GameScene::DrawBackSprite()
{
}

void GameScene::DrawModel()
{
	obj.Draw();
	obj2.Draw();
}

void GameScene::DrawFrontSprite()
{
	spr.Draw();
}

void GameScene::DrawDebugGui()
{
	//GuiManager::BeginWindow();

	//GuiManager::EndWindow();
}
