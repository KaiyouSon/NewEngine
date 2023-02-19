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

	isCG3 = true;
	isAL4 = false;

	//renderTexture.pos = GetWindowHalfSize();

	CollisionInit();
	CG3Init();
}
void GameScene::Update()
{
	//renderTexture.Update();

	if (isCG3 == true)
	{
		CG3Update();
	}
	if (isAL4 == true)
	{
		CollisionUpdate();
	}

	Quaternion q = Quaternion::DirectionToDirection({ 1.f,0.f,1.f }, { 1.f,1.f,0.f });

	Camera::DebugCameraUpdate();
	//if (Key::GetKey(DIK_SPACE))
	//{
	//	SceneManager::ChangeScene<TitleScene>();
	//}
}

void GameScene::DrawRenderTexture()
{
	//renderTexture.PreDrawScene();
	//CG3DrawModel();
	//renderTexture.PostDrawScene();
}
void GameScene::DrawBackSprite()
{
}
void GameScene::DrawModel()
{
	if (isCG3 == true)
	{
		CG3DrawModel();
	}
	if (isAL4 == true)
	{
		CollisionDrawModel();
	}
}
void GameScene::DrawFrontSprite()
{
	//renderTexture.Draw();
}
void GameScene::DrawDebugGui()
{
	GuiManager::BeginWindow("Main");
	GuiManager::DrawCheckBox("CG3", &isCG3);
	GuiManager::DrawTab();
	GuiManager::DrawCheckBox("AL4", &isAL4);
	GuiManager::EndWindow();

	GuiManager::BeginWindow("Camera Option");
	GuiManager::DrawSlider3("Camera Pos", Camera::current.pos, 0.01f);
	GuiManager::DrawSlider3("Camera Rot", Camera::current.rot, 0.01f);
	GuiManager::EndWindow();

	CG3DrawGui();
	CollisionDrawGui();
}

void GameScene::CollisionInit()
{
	currentCollision = 0;

	sphereObj.model = Model("Sphere");

	planeObj.model = Model("Ground");

	triangleObj.model = Model("Triangle");

	rayObj.model = Model("Ray");
	rayObj.pos.y = 5;
	rayObj.scale.y = 100;

	capsuleObj1.model = Model("Capsule");
	capsuleObj1.pos.x = +2;
	capsuleObj2.model = Model("Capsule");
	capsuleObj2.pos.x = -2;
}
void GameScene::CollisionUpdate()
{
	/*if (currentCollision == 0)
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
			triangleObj.pos + Vec3(-1.f,+1.f,0.f),
			triangleObj.pos + Vec3(-1.f,-1.f,0.f),
			triangleObj.pos + Vec3(+1.f,-1.f,0.f),
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
	else if (currentCollision == 3)
	{
		RayCollider c1 = { rayObj.pos,{0,-1,0} };
		SphereCollider c2 = { sphereObj.pos,sphereObj.scale.x };

		if (Collision::RayHitSphere(c1, c2) == true)
		{
			rayObj.color = Color::red;
			sphereObj.color = Color::red;
		}
		else
		{
			rayObj.color = Color::white;
			sphereObj.color = Color::white;
		}
	}
	else if (currentCollision == 4)
	{
		CapsuleCollider c1 =
		{
			capsuleObj1.pos + Vec3(0.f,+1.f,0.f),
			capsuleObj1.pos + Vec3(0.f,-1.f,0.f),
			1
		};
		CapsuleCollider c2 =
		{
			capsuleObj2.pos + Vec3(0.f,+1.f,0.f),
			capsuleObj2.pos + Vec3(0.f,-1.f,0.f),
			1
		};

		if (Collision::CapsuleHitCapsule(c1, c2) == true)
		{
			capsuleObj1.color = Color::red;
			capsuleObj2.color = Color::red;
		}
		else
		{
			capsuleObj1.color = Color::white;
			capsuleObj2.color = Color::white;
		}
	}*/

	sphereObj.Update();
	planeObj.Update();
	triangleObj.Update();
	rayObj.Update();

	capsuleObj1.Update();
	capsuleObj2.Update();
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
	else if (currentCollision == 3)
	{
		rayObj.Draw();
		sphereObj.Draw();
	}
	else if (currentCollision == 4)
	{
		capsuleObj1.Draw();
		capsuleObj2.Draw();
	}
}
void GameScene::CollisionDrawGui()
{
	GuiManager::BeginWindow("Collision Debug");
	GuiManager::DrawRadioButton("Sphere Hit Plane", currentCollision, 0, false);
	GuiManager::DrawRadioButton("Sphere Hit Triangle", currentCollision, 1, false);
	GuiManager::DrawRadioButton("Ray Hit Plane", currentCollision, 2, false);
	GuiManager::DrawRadioButton("Ray Hit Sphere", currentCollision, 3, false);
	GuiManager::DrawRadioButton("Capsule Hit Capsule", currentCollision, 4, false);

	if (currentCollision == 0)
	{
		GuiManager::DrawSlider3("sphere pos", sphereObj.pos, 0.01f);
		GuiManager::DrawSlider3("mesh pos", planeObj.pos, 0.01f);
	}
	else if (currentCollision == 1)
	{
		GuiManager::DrawSlider3("sphere pos", sphereObj.pos, 0.01f);
		GuiManager::DrawSlider3("triangle pos", triangleObj.pos, 0.01f);
	}
	else if (currentCollision == 2)
	{
		GuiManager::DrawSlider3("ray pos", rayObj.pos, 0.01f);
		GuiManager::DrawSlider3("mesh pos", planeObj.pos, 0.01f);
	}
	else if (currentCollision == 3)
	{
		GuiManager::DrawSlider3("ray pos", rayObj.pos, 0.01f);
		GuiManager::DrawSlider3("sphere pos", sphereObj.pos, 0.01f);
	}
	else if (currentCollision == 4)
	{
		GuiManager::DrawSlider3("capsule1 pos", capsuleObj1.pos, 0.01f);
		GuiManager::DrawSlider3("capsule2 pos", capsuleObj2.pos, 0.01f);
	}

	GuiManager::EndWindow();
}

void GameScene::CG3Init()
{
	LightManager::isPointLighting = true;
	LightManager::isSpotLighting = true;

	LightManager::GetInstance()->directionalLights[0].isActive = true;
	LightManager::GetInstance()->circleShadow.isActive = true;
	LightManager::GetInstance()->circleShadow.vec = Vec3::down;
	LightManager::GetInstance()->circleShadow.atten = { 0.5f,0.6f,0.0f };
	LightManager::GetInstance()->circleShadow.factorAngleCos = { 0.0f,0.5f };

	skyDomeObj.model = *ModelManager::GetModel("SkyDome");
	groundObj.model = *ModelManager::GetModel("Ground");
	groundObj.pos.y = -1.f;

	obj.model = *ModelManager::GetModel("AttackEnemy");
	obj.pos = { 1.5f,0.f,5.f };

	obj2.model = *ModelManager::GetModel("Player");
	obj2.pos.y = 1.5f;

	obj3.model = *ModelManager::GetModel("Enemy");
	obj3.pos = { -3.f,2.f,0.f };

	silhouetteObj.obj = &obj;
	outlineObj.obj = &obj2;
}
void GameScene::CG3Update()
{
	obj3.rot.y += Radian(1);

	obj3.Update();
	outlineObj.Update();
	silhouetteObj.Update();
	skyDomeObj.Update();
	groundObj.Update();

	LightManager::GetInstance()->circleShadow.pos = obj3.pos;
}
void GameScene::CG3DrawModel()
{
	obj3.Draw();
	//outlineObj.Draw();
	//groundObj.Draw();
	//skyDomeObj.Draw();
	//silhouetteObj.Draw();
}
void GameScene::CG3DrawGui()
{
	CG3DrawModelGui();
	DirectionalLightDrawGui();
	PointLightDrawGui();
	SpotLightDrawGui();
	CircleDrawGui();
	FogDrawGui();
}
void GameScene::CG3DrawModelGui()
{
	GuiManager::BeginWindow("Model");
	GuiManager::DrawSlider3("Enemy pos", obj.pos, 0.01f);
	GuiManager::DrawSlider3("Player pos", obj2.pos, 0.01f);
	GuiManager::DrawSlider3("Sphere pos", obj3.pos, 0.01f);
	GuiManager::EndWindow();
}

void GameScene::DirectionalLightDrawGui()
{
	GuiManager::BeginWindow("DirectionalLight");

	GuiManager::DrawCheckBox("DirectionalLight isActive", &LightManager::GetInstance()->directionalLights[0].isActive);
	GuiManager::DrawSlider3("DirectionalLight pos", LightManager::GetInstance()->directionalLights[0].dirVec);
	GuiManager::DrawColorEdit("DirectionalLight color", LightManager::GetInstance()->directionalLights[0].color);

	GuiManager::EndWindow();
}
void GameScene::PointLightDrawGui()
{
	GuiManager::BeginWindow("PointLight");

	for (int i = 0; i < 3; i++)
	{
		std::string str;

		str = "PointLight isActive" + std::to_string(i);
		GuiManager::DrawCheckBox(str.c_str(), &LightManager::GetInstance()->pointLights[i].isActive);

		str = "PointLight pos" + std::to_string(i);
		GuiManager::DrawSlider3(str.c_str(), LightManager::GetInstance()->pointLights[i].pos);

		str = "PointLight atten" + std::to_string(i);
		GuiManager::DrawSlider3(str.c_str(), LightManager::GetInstance()->pointLights[i].atten);

		str = "PointLight color" + std::to_string(i);
		GuiManager::DrawColorEdit(str.c_str(), LightManager::GetInstance()->pointLights[i].color);
	}

	GuiManager::EndWindow();
}
void GameScene::SpotLightDrawGui()
{
	GuiManager::BeginWindow("SpotLight");

	for (int i = 0; i < 3; i++)
	{
		std::string str;

		str = "SpotLight isActive" + std::to_string(i);
		GuiManager::DrawCheckBox(str.c_str(), &LightManager::GetInstance()->spotLights[i].isActive);

		str = "SpotLight pos" + std::to_string(i);
		GuiManager::DrawSlider3(str.c_str(), LightManager::GetInstance()->spotLights[i].pos);

		str = "SpotLight vec" + std::to_string(i);
		GuiManager::DrawSlider3(str.c_str(), LightManager::GetInstance()->spotLights[i].vec);

		str = "SpotLight atten" + std::to_string(i);
		GuiManager::DrawSlider3(str.c_str(), LightManager::GetInstance()->spotLights[i].atten);

		str = "SpotLight factor angle cos" + std::to_string(i);
		GuiManager::DrawSlider2(str.c_str(), LightManager::GetInstance()->spotLights[i].factorAngle);

		str = "SpotLight color" + std::to_string(i);
		GuiManager::DrawColorEdit(str.c_str(), LightManager::GetInstance()->spotLights[i].color);
	}

	GuiManager::EndWindow();
}
void GameScene::CircleDrawGui()
{
	GuiManager::BeginWindow("CircleShadow");
	GuiManager::DrawCheckBox("CircleShadow isActive", &LightManager::GetInstance()->circleShadow.isActive);
	GuiManager::DrawSlider3("CircleShadow vec", LightManager::GetInstance()->circleShadow.vec);
	GuiManager::DrawSlider3("CircleShadow atten", LightManager::GetInstance()->circleShadow.atten);
	GuiManager::DrawSlider2("CircleShadow factorAngleCos", LightManager::GetInstance()->circleShadow.factorAngleCos);
	GuiManager::DrawSlider1("CircleShadow disCasterLight", LightManager::GetInstance()->circleShadow.disCasterLight);
	GuiManager::EndWindow();
}
void GameScene::FogDrawGui()
{
	GuiManager::BeginWindow("Fog");
	GuiManager::DrawCheckBox("Fog isActive", &LightManager::GetInstance()->fog.isActive);
	GuiManager::DrawSlider1("Fog nearDis", LightManager::GetInstance()->fog.nearDis);
	GuiManager::DrawSlider1("Fog farDis", LightManager::GetInstance()->fog.farDis);
	GuiManager::DrawColorEdit("Fog Color", LightManager::GetInstance()->fog.color);
	GuiManager::EndWindow();
}

void GameScene::QuaternionDrawGui()
{

	Quaternion q1 = { 2.f,3.f,4.f,1.f };
	Quaternion q2 = { 1.f,3.f,5.f,2.f };

	Quaternion mul1 = q1 * q2;
	Quaternion mul2 = q2 * q1;
	GuiManager::BeginWindow("Quaternion");

	//ImGui::Text("%f,%f,%f,%f : Identity",
	//	Quaternion::Identity().x, Quaternion::Identity().y, Quaternion::Identity().z, Quaternion::Identity().w);

	//ImGui::Text("%f,%f,%f,%f : Conjugate",
	//	q1.Conjugate().x, q1.Conjugate().y, q1.Conjugate().z, q1.Conjugate().w);

	//ImGui::Text("%f,%f,%f,%f Inverse",
	//	q1.Inverse().x, q1.Inverse().y, q1.Inverse().z, q1.Inverse().w);

	//ImGui::Text("%f,%f,%f,%f : Normalize",
	//	q1.Norm().x, q1.Norm().y, q1.Norm().z, q1.Norm().w);

	//ImGui::Text("%f,%f,%f,%f : mul1",
	//	mul1.x, mul1.y, mul1.z, mul1.w);

	//ImGui::Text("%f,%f,%f,%f : mul2",
	//	mul2.x, mul2.y, mul2.z, mul2.w);

	//ImGui::Text("%f : Length", q1.Length());

	GuiManager::EndWindow();

}

