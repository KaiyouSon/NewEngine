#include "GameScene.h"
#include "TitleScene.h"
#include "LightManager.h"

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
	//DirectionalLight::current.pos = { 1,1,1 };

	LightManager::GetInstance()->Init();
	LightManager::GetInstance()->directionalLights[0].isActive = false;
	LightManager::GetInstance()->directionalLights[1].isActive = false;
	LightManager::GetInstance()->directionalLights[2].isActive = false;

	Object3D::isAllLighting = true;

	skyDomeObj.model = Model("SkyDome", true);
	groundObj.model = Model("Ground");
	groundObj.pos.y = -2;

	Texture tex = Texture("pic.png");
	obj.model = Model("bee", true);
	obj.pos.x = -2.f;

	obj2.model = Model("player", true);
	obj2.pos.x = 2.f;

	spr.texture = tex;
	spr.scale = 0.25f;
	spr.anchorPoint = 0.5f;

}

void GameScene::Update()
{
	LightManager::GetInstance()->pointLights[0].pos = { 0.5f, 1.0f, 0.0f };
	LightManager::GetInstance()->pointLights[1].pos = { 0.5f, 1.0f, 0.0f };
	LightManager::GetInstance()->pointLights[2].pos = { 0.5f, 1.0f, 0.0f };

	LightManager::GetInstance()->pointLights[0].color = 1.0f;
	LightManager::GetInstance()->pointLights[1].color = 1.0f;
	LightManager::GetInstance()->pointLights[2].color = 1.0f;

	LightManager::GetInstance()->pointLights[0].atten = { 0.3f, 0.1f ,0.1f };
	LightManager::GetInstance()->pointLights[1].atten = { 0.3f, 0.1f ,0.1f };
	LightManager::GetInstance()->pointLights[2].atten = { 0.3f, 0.1f ,0.1f };

	//LightManager::GetInstance()->pointLights[0].isActive = true;
	//LightManager::GetInstance()->pointLights[1].isActive = true;
	LightManager::GetInstance()->pointLights[2].isActive = true;


	obj.Update();
	obj2.Update();
	spr.Update();

	skyDomeObj.Update();
	groundObj.Update();

	Camera::DebugCameraUpdate();
	LightManager::GetInstance()->Update();

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
