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
	LightManager::GetInstance()->directionalLights[0].isActive = false;
	LightManager::GetInstance()->directionalLights[1].isActive = false;
	LightManager::GetInstance()->directionalLights[2].isActive = false;

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

	LightManager::GetInstance()->spotLights[0].isActive = false;
	LightManager::GetInstance()->spotLights[1].isActive = false;
	LightManager::GetInstance()->spotLights[2].isActive = true;

	// LightManager::GetInstance()->spotLights[2].vec = Vec3::down;
	// LightManager::GetInstance()->spotLights[2].pos = { 0,5,0 };
	// LightManager::GetInstance()->spotLights[2].color = 1.f;
	// LightManager::GetInstance()->spotLights[2].atten = Vec3::zero;
	// LightManager::GetInstance()->spotLights[2].factorAngleCos = { 20.f,30.f };

	CollisionInit();
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


	CollisionUpdate();
}

void GameScene::DrawBackSprite()
{
}

void GameScene::DrawModel()
{
	//obj.Draw();
	//obj2.Draw();
	//skyDomeObj.Draw();
	//groundObj.Draw();

	CollisionDrawModel();
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

	CollisionDrawGui();

	//GuiManager::BeginWindow("SpotLight");

	//GuiManager::DrawSlider3("vec", LightManager::GetInstance()->spotLights[2].vec);
	//GuiManager::DrawSlider3("pos", LightManager::GetInstance()->spotLights[2].pos);
	//GuiManager::DrawSlider3("color", LightManager::GetInstance()->spotLights[2].color);
	//GuiManager::DrawSlider3("atten", LightManager::GetInstance()->spotLights[2].atten);
	//GuiManager::DrawSlider2("factor angle cos", LightManager::GetInstance()->spotLights[2].factorAngleCos);

	//GuiManager::EndWindow();
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
