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
	Camera::current.pos = { 0,0,-15 };
	Camera::current.rot = { Radian(0),0,0 };
	Light::current.lightPos = { 1,1,1 };

	skyDomeObj.model = Model("SkyDome", true);
	groundObj.model = Model("Ground");
	groundObj.pos.y = -5;

	Texture tex = Texture("pic.png");
	obj.model = Model("bee", true);
	obj.pos.z = -5;

	obj2.model = Model("player", true);
	obj2.pos.z = 0;

	spr.texture = tex;
	spr.scale = 0.25f;
	spr.anchorPoint = 0.5f;

}

void GameScene::Update()
{
	obj.Update();
	obj2.Update();
	spr.Update();

	skyDomeObj.Update();
	groundObj.Update();

	Camera::DebugCameraUpdate();
}

void GameScene::DrawBackSprite()
{
}

void GameScene::DrawModel()
{
	obj.Draw();
	obj2.Draw();
	skyDomeObj.Draw();
	groundObj.Draw();
}

void GameScene::DrawFrontSprite()
{
	spr.Draw();
}

void GameScene::DrawDebugGui()
{
	Quaternion q = { 0,1,0 };

	Vec3 v;
	v = q.AnyAxisRotation({ 0,0,1 }, Radian(90));

	Quaternion q2;
	q2 = q * q.Inverse();
	//v = v * v1;

	Mat4 m = ConvertRotationMat(q.AnyAxisRotation({ 0,0,1 }, Radian(90)));
	m.SetTranslation(v);
	Vec3 v2 = m.ExtractTranslation();

	GuiManager::BeginWindow();

	GuiManager::DrawString("%f,%f,%f", v.x, v.y, v.z);
	GuiManager::DrawString("%f,%f,%f", v2.x, v2.y, v2.z);

	GuiManager::EndWindow();
}
