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

	//spr.texture = tex;
	//spr.scale = 0.25f;
	//spr.anchorPoint = 0.5f;

}

void GameScene::Update()
{
	static int angle = 0;
	angle++;
	if (angle > 360) angle = 1;

	Quaternion q = { 10,0,0 };
	obj.pos = q.AnyAxisRotation({ 0,0,1 }, angle);

	//obj2.Update();

	//obj.rot.y -= Radian(3);
	//obj2.rot.y -= Radian(3);

	obj.Update();
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

	//spr.Update();

	Camera::DebugCameraUpdate();
}

void GameScene::DrawBackSprite()
{
}

void GameScene::DrawModel()
{
	obj.Draw();
	//obj2.Draw();
}

void GameScene::DrawFrontSprite()
{
	//spr.Draw();
}

void GameScene::DrawDebugGui()
{
	// 座標(0,5,0)のクォータニオン作成
	Quaternion p = { 5,0,0 };
	Quaternion q = p.AnyAxisRotation({ 1,0,1 }, 180);

	GuiManager::BeginWindow();

	//GuiManager::DrawString("q = (%f, %f, %f)", q.x, q.y, q.z);

	GuiManager::EndWindow();
}
