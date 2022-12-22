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
	obj.pos.x = 3;

	spr.texture = tex;
	spr.scale = 0.25f;
	spr.anchorPoint = 0.5f;

}

void GameScene::Update()
{
	obj.Update();
	spr.Update();

	Camera::DebugCameraUpdate();
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
	spr.Draw();
}

void GameScene::DrawDebugGui()
{
	//GuiManager::BeginWindow();

	//GuiManager::EndWindow();
}
