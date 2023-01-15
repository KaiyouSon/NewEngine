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


	LightManager::GetInstance()->spotLights[0].isActive = false;
	LightManager::GetInstance()->spotLights[1].isActive = false;
	LightManager::GetInstance()->spotLights[2].isActive = true;

	//LightManager::GetInstance()->spotLights[2].vec = Vec3::down;
	//LightManager::GetInstance()->spotLights[2].pos = { 0,5,0 };
	//LightManager::GetInstance()->spotLights[2].color = 1.f;
	//LightManager::GetInstance()->spotLights[2].atten = Vec3::zero;
	//LightManager::GetInstance()->spotLights[2].factorAngleCos = { 20.f,30.f };
}

void GameScene::Update()
{
	LightManager::GetInstance()->pointLights[0].pos = { 0.5f, 1.0f, 0.0f };
	LightManager::GetInstance()->pointLights[1].pos = { 0.5f, 1.0f, 0.0f };
	LightManager::GetInstance()->pointLights[2].pos = { 0.5f, 1.0f, 0.0f };

	LightManager::GetInstance()->pointLights[0].color = 1.f;
	LightManager::GetInstance()->pointLights[1].color = 1.f;
	LightManager::GetInstance()->pointLights[2].color = 1.f;

	LightManager::GetInstance()->pointLights[0].atten = { 0.3f, 0.1f ,0.1f };
	LightManager::GetInstance()->pointLights[1].atten = { 0.3f, 0.1f ,0.1f };
	LightManager::GetInstance()->pointLights[2].atten = { 0.3f, 0.1f ,0.1f };

	LightManager::GetInstance()->pointLights[0].isActive = false;
	LightManager::GetInstance()->pointLights[1].isActive = false;
	LightManager::GetInstance()->pointLights[2].isActive = false;

	obj.rot.y += Radian(2);
	obj2.rot.y += Radian(2);

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
	//	Quaternion q = { 0,1,0 };
	//
	//	Vec3 v;
	//	v = q.AnyAxisRotation({ 0,0,1 }, Radian(90));
	//
	//	Quaternion q2;
	//	q2 = q * q.Inverse();
	//	//v = v * v1;
	//
	//	Mat4 m = ConvertRotationMat(q.AnyAxisRotation({ 0,0,1 }, Radian(90)));
	//	m.SetTranslation(v);
	//	Vec3 v2 = m.ExtractTranslation();
	//
	//	GuiManager::BeginWindow();
	//
	//	GuiManager::DrawString("%f,%f,%f", v.x, v.y, v.z);
	//	GuiManager::DrawString("%f,%f,%f", v2.x, v2.y, v2.z);
	//
	//	GuiManager::EndWindow();

	GuiManager::BeginWindow("SpotLight");

	GuiManager::DrawSlider3("vec", LightManager::GetInstance()->spotLights[2].vec);
	GuiManager::DrawSlider3("pos", LightManager::GetInstance()->spotLights[2].pos);
	GuiManager::DrawSlider3("color", LightManager::GetInstance()->spotLights[2].color);
	GuiManager::DrawSlider3("atten", LightManager::GetInstance()->spotLights[2].atten);
	GuiManager::DrawSlider2("factor angle cos", LightManager::GetInstance()->spotLights[2].factorAngleCos);

	GuiManager::EndWindow();
}
