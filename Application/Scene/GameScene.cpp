#include "GameScene.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "NewEngine.h"

GameScene::GameScene()
{
}
GameScene::~GameScene()
{
}

void GameScene::Init()
{
	Camera::current.pos = { 0,0,-30 };

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
