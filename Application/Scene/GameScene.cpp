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
	Light::GetCurrent()->Init();

	Model model = Model("sphere", true);
	Texture tex = Texture("pic.png");

	obj.model = model;
}

void GameScene::Update()
{
	obj.Update();
	obj.color = Color::red;

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
}

void GameScene::DrawDebugGui()
{
	//GuiManager::BeginWindow();

	//GuiManager::EndWindow();
}
