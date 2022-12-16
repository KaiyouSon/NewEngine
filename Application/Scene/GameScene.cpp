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
	//Camera::current.pos = { 0,0,0 };
	//Camera::current.rot = { 0,0,0 };
	Light::GetCurrent()->Init();
	//Light::GetCurrent()->lightDir = Vec3::down;

	Model model = Model("sphere", true);
	Texture tex = Texture("pic.png");

	obj.model = model;
}

void GameScene::Update()
{
	obj.Update();

	//if (Key::GetKey(DIK_RIGHT))
	//{
	//	obj.rot.y -= Radian(5);
	//}
	//if (Key::GetKey(DIK_LEFT))
	//{
	//	obj.rot.y += Radian(5);
	//}

	//if (Key::GetKey(DIK_UP))
	//{
	//	obj.rot.x += Radian(5);
	//	//obj.color.r++;
	//}
	//if (Key::GetKey(DIK_DOWN))
	//{
	//	obj.rot.x -= Radian(5);
	//	//obj.color.r--;
	//}

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
