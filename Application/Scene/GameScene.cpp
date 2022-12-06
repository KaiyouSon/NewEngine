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
	Model test = Model("Cube");
	Texture tex = Texture("pic.png");
	for (int i = 0; i < 10; i++)
	{
		obj[i].model = test;
		obj[i].texture = tex;
	}

	spr.texture = tex;

}

void GameScene::Update()
{
	for (int i = 0; i < 10; i++)
	{
		const float offsetAngle = (360 / 10) * i;
		obj[i].pos = { sinf(Radian(offsetAngle)) * 30, 0, cosf(Radian(offsetAngle)) * 30 };
		obj[i].rot.y = offsetAngle;
		obj[i].Update();
	}
	spr.Update();

	if (Key::GetKeyTrigger(DIK_SPACE))
	{
		SceneManager::ChangeScene<TitleScene>();
	}

	ImGui::Begin("Debug");

	ImGui::Text("pos.z = %f", obj[0].pos.z);
	ImGui::End();
}

void GameScene::DrawBackSprite()
{
}

void GameScene::DrawModel()
{
	for (int i = 0; i < 10; i++)
	{
		obj[i].Draw();
	}
}

void GameScene::DrawFrontSprite()
{
}