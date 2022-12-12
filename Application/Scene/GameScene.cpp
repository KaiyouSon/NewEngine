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

	m1 = Model("player");
	m2 = Model("bee");

	for (int i = 0; i < 10; i++)
	{
		obj[i].model = test;
		//obj[i].texture = tex;
	}

	spr.texture = tex;

}

void GameScene::Update()
{
	for (int i = 0; i < 10; i++)
	{
		const float offsetAngle = (360 / 10) * i;
		obj[i].pos = { sinf(Radian(offsetAngle)) * 30, 0, cosf(Radian(offsetAngle)) * 30 };
		//obj[i].rot.y = offsetAngle;
		obj[i].Update();
	}
	spr.Update();

	Camera::DebugCameraUpdate();

	if (Key::GetKeyTrigger(DIK_SPACE))
	{
		SceneManager::ChangeScene<TitleScene>();
	}

	if (Key::GetKeyTrigger(DIK_1))
	{
		for (int i = 0; i < 10; i++)
		{
			obj[i].model = m1;
		}
	}

	if (Key::GetKeyTrigger(DIK_2))
	{
		for (int i = 0; i < 10; i++)
		{
			obj[i].model = m2;
		}
	}
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

void GameScene::DrawDebugGui()
{
	GuiManager::BeginWindow();

	//GuiManager::DrawString("%f", obj[0].pos.z);
	GuiManager::DrawSlider("test", obj[0].pos);

	GuiManager::EndWindow();
}
