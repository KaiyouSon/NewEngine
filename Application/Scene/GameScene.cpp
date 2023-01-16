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
	Camera::current.pos = { 0,1,-15 };
	Camera::current.rot = { Radian(0),0,0 };
	//DirectionalLight::current.pos = { 1,1,1 };

	LightManager::GetInstance()->Init();
	LightManager::GetInstance()->directionalLights[2].isActive = false;

	LightManager::GetInstance()->spotLights[2].isActive = false;
	LightManager::GetInstance()->spotLights[2].vec = Vec3::down;
	LightManager::GetInstance()->spotLights[2].pos = { 0,5,0 };
	LightManager::GetInstance()->spotLights[2].color = Color::white;
	LightManager::GetInstance()->spotLights[2].atten = Vec3::zero;
	LightManager::GetInstance()->spotLights[2].factorAngleCos = { 20.f,30.f };

	LightManager::GetInstance()->pointLights[2].isActive = true;
	LightManager::GetInstance()->pointLights[2].pos = { 0.5f, 1.0f, 0.0f };
	LightManager::GetInstance()->pointLights[2].color = Color::white;
	LightManager::GetInstance()->pointLights[2].atten = { 0.3f, 0.1f ,0.1f };

	LightManager::GetInstance()->circleShadow.isActive = true;
	LightManager::GetInstance()->circleShadow.vec = { 0,-1,0 };
	LightManager::GetInstance()->circleShadow.atten = { 0.5f,0.6f,0.0f };
	LightManager::GetInstance()->circleShadow.factorAngleCos = { 0.0f,0.5f };

	skyDomeObj.model = Model("SkyDome", true);
	groundObj.model = Model("Ground");
	groundObj.pos.y = -2;

	Texture tex = Texture("pic.png");
	obj.model = Model("bee", true);
	obj.pos.x = -2.f;

	obj2.model = Model("Sphere", true);
	obj2.pos.x = 2.f;

	spr.texture = tex;
	spr.scale = 0.25f;
	spr.anchorPoint = 0.5f;

	triangleObj.model = Model("Triangle");

	CollisionInit();
}
void GameScene::Update()
{
	obj.rot.y += Radian(2);
	obj2.rot.y += Radian(2);

	obj.Update();
	obj2.Update();
	spr.Update();

	skyDomeObj.Update();
	groundObj.Update();

	LightManager::GetInstance()->circleShadow.pos = obj2.pos;

	Camera::DebugCameraUpdate();
	LightManager::GetInstance()->Update();

	//CollisionUpdate();
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

	//CollisionDrawModel();
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

	DirectionalLightDrawGui();
	PointLightDrawGui();
	SpotLightDrawGui();

	CollisionDrawGui();
}

void GameScene::CollisionInit()
{
	currentCollision = 0;

	sphereObj.model = Model("Sphere");

	planeObj.model = Model("Ground");

	rayObj.model = Model("Ray");
	rayObj.pos.y = 5;
	rayObj.scale.y = 100;
	//rayObj.scale = { 0.1f,1,1f };
}
void GameScene::CollisionUpdate()
{
	if (currentCollision == 0)
	{
		SphereCollider c1 = { sphereObj.pos,sphereObj.scale.x };
		PlaneCollider c2 = { planeObj.pos,{0,1,0} };
		if (Collision::SphereHitPlane(c1, c2) == true)
		{
			sphereObj.color = Color::red;
		}
		else
		{
			sphereObj.color = Color::white;
		}
	}
	else if (currentCollision == 1)
	{
		SphereCollider c1 = { sphereObj.pos,sphereObj.scale.x };
		TriangleCollider c2 =
		{
			triangleObj.pos + Vec3(-1,1,0),
			triangleObj.pos + Vec3(-1,-1,0),
			triangleObj.pos + Vec3(1,-1,0),
			Vec3(0,0,-1)
		};

		if (Collision::SphereHitTriangle(c1, c2) == true)
		{
			sphereObj.color = Color::red;
			triangleObj.color = Color::red;
		}
		else
		{
			sphereObj.color = Color::white;
			triangleObj.color = Color::white;
		}
	}
	else if (currentCollision == 2)
	{
		RayCollider c1 = { rayObj.pos,{0,-1,0} };
		PlaneCollider c2 = { planeObj.pos,{0,1,0} };

		if (Collision::RayHitPlane(c1, c2) == true)
		{
			rayObj.color = Color::red;
		}
		else
		{
			rayObj.color = Color::white;
		}
	}

	if (currentCollision == 0)
	{
		sphereObj.Update();
		planeObj.Update();
	}
	else if (currentCollision == 1)
	{
		sphereObj.Update();
		triangleObj.Update();
	}
	else if (currentCollision == 2)
	{
		rayObj.Update();
		planeObj.Update();
	}
}
void GameScene::CollisionDrawModel()
{
	if (currentCollision == 0)
	{
		sphereObj.Draw();
		planeObj.Draw();
	}
	else if (currentCollision == 1)
	{
		sphereObj.Draw();
		triangleObj.Draw();
	}
	else if (currentCollision == 2)
	{
		rayObj.Draw();
		planeObj.Draw();
	}
}
void GameScene::CollisionDrawGui()
{
	GuiManager::BeginWindow("Collision Debug");
	GuiManager::DrawRadioButton("Radio Button A", currentCollision, 0, false);
	GuiManager::DrawRadioButton("Radio Button B", currentCollision, 1, false);
	GuiManager::DrawRadioButton("Radio Button C", currentCollision, 2, false);

	if (currentCollision == 0)
	{
		GuiManager::DrawSlider3("sphere pos", sphereObj.pos, 0.01f);
	}
	else if (currentCollision == 1)
	{
		GuiManager::DrawSlider3("sphere pos", sphereObj.pos, 0.01f);
		GuiManager::DrawSlider3("triangle pos", triangleObj.pos, 0.01f);
	}
	else if (currentCollision == 2)
	{
		GuiManager::DrawSlider3("rayObj pos", rayObj.pos, 0.01f);
	}

	GuiManager::EndWindow();

	GuiManager::BeginWindow("Camera Option");
	GuiManager::DrawSlider3("Camera Pos", Camera::current.pos, 0.01f);
	GuiManager::DrawSlider3("Camera Rot", Camera::current.rot, 0.01f);
	GuiManager::EndWindow();
}

void GameScene::DirectionalLightDrawGui()
{
	GuiManager::BeginWindow("DirectionalLight");

	GuiManager::DrawCheckBox("DirectionalLight isActive", &LightManager::GetInstance()->directionalLights[2].isActive);
	GuiManager::DrawSlider3("DirectionalLight pos", LightManager::GetInstance()->directionalLights[2].pos);
	GuiManager::DrawColorEdit("DirectionalLight color", LightManager::GetInstance()->directionalLights[2].color);

	GuiManager::EndWindow();
}
void GameScene::PointLightDrawGui()
{
	GuiManager::BeginWindow("PointLight");

	GuiManager::DrawCheckBox("PointLight isActive", &LightManager::GetInstance()->pointLights[2].isActive);
	GuiManager::DrawSlider3("PointLight pos", LightManager::GetInstance()->pointLights[2].pos);
	GuiManager::DrawSlider3("PointLight atten", LightManager::GetInstance()->pointLights[2].atten);
	GuiManager::DrawColorEdit("PointLight color", LightManager::GetInstance()->pointLights[2].color);

	GuiManager::EndWindow();
}
void GameScene::SpotLightDrawGui()
{
	GuiManager::BeginWindow("SpotLight");

	GuiManager::DrawCheckBox("SpotLight isActive", &LightManager::GetInstance()->spotLights[2].isActive);
	GuiManager::DrawSlider3("SpotLight vec", LightManager::GetInstance()->spotLights[2].vec);
	GuiManager::DrawSlider3("SpotLight pos", LightManager::GetInstance()->spotLights[2].pos);
	GuiManager::DrawSlider3("SpotLight atten", LightManager::GetInstance()->spotLights[2].atten);
	GuiManager::DrawSlider2("SpotLight factor angle cos", LightManager::GetInstance()->spotLights[2].factorAngleCos);
	GuiManager::DrawColorEdit("SpotLight color", LightManager::GetInstance()->spotLights[2].color);

	GuiManager::EndWindow();
}
