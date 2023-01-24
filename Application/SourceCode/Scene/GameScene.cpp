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

	LightManager::isPointLighting = false;
	LightManager::isSpotLighting = false;

	LightManager::GetInstance()->circleShadow.isActive = false;
	LightManager::GetInstance()->circleShadow.vec = { 0,-1,0 };
	LightManager::GetInstance()->circleShadow.atten = { 0.5f,0.6f,0.0f };
	LightManager::GetInstance()->circleShadow.factorAngleCos = { 0.0f,0.5f };

	ModelManager::LoadModel("SkyDome", "SkyDome");
	ModelManager::LoadModel("Ground", "Ground");
	ModelManager::LoadModel("Boss", "Boss", true);
	ModelManager::LoadModel("Sphere", "Sphere", true);
	ModelManager::LoadModel("Cube", "Cube", true);
	ModelManager::LoadModel("player", "Player", true);


	TextureManager::LoadTexture("pic.png", "pic");

	skyDomeObj.model = *ModelManager::GetModel("SkyDome");
	groundObj.model = *ModelManager::GetModel("Ground");
	groundObj.pos.y = -2;

	obj.model = *ModelManager::GetModel("Boss");
	obj.texture = *TextureManager::GetTexture("pic");
	obj.pos.z = 5.f;

	obj2.model = *ModelManager::GetModel("Player");
	//obj2.color = Color::red;

	spr.texture = *TextureManager::GetTexture("pic");
	spr.pos = 256;
	spr.scale = 0.25f;
	spr.anchorPoint = 0.5f;

	silhouetteObj.obj = &obj;
	outlineObj.obj = &obj2;

	CollisionInit();
}
void GameScene::Update()
{
	//obj.rot.y += Radian(2);
	//obj2.rot.y += Radian(2);

	spr.SetTextureRect(408, 816);

	//obj.Update();
	//obj2.Update();
	spr.Update();

	//obj2.rot.y += Radian(1);

	outlineObj.Update();
	silhouetteObj.Update();

	skyDomeObj.Update();
	groundObj.Update();

	LightManager::GetInstance()->circleShadow.pos = obj2.pos;

	Camera::DebugCameraUpdate();
	LightManager::GetInstance()->Update();

	CollisionUpdate();

	Quaternion q3 = Quaternion::MakeAxisAngle({ 0.71f,0.71f,0.0f }, 0.3f);
	Quaternion q4 = -q3;

	Quaternion i0 = Quaternion::Slerp(q3, q4, 0.0f);
	Quaternion i1 = Quaternion::Slerp(q3, q4, 0.3f);
	Quaternion i2 = Quaternion::Slerp(q3, q4, 0.5f);
	Quaternion i3 = Quaternion::Slerp(q3, q4, 0.7f);
	Quaternion i4 = Quaternion::Slerp(q3, q4, 1.0f);

	if (Key::GetKey(DIK_SPACE))
	{
		SceneManager::ChangeScene<TitleScene>();
	}
}

void GameScene::DrawBackSprite()
{
}
void GameScene::DrawModel()
{
	//CollisionDrawModel();

	outlineObj.Draw();
	groundObj.Draw();
	//silhouetteObj.Draw();
}
void GameScene::DrawFrontSprite()
{
	spr.Draw();
}
void GameScene::DrawDebugGui()
{
	DebugGui();
	DirectionalLightDrawGui();
	PointLightDrawGui();
	SpotLightDrawGui();
	CollisionDrawGui();
}

void GameScene::DebugGui()
{
	GuiManager::BeginWindow("Debug");

	GuiManager::DrawSlider3("Sphere pos", obj.pos, 0.01f);
	GuiManager::DrawSlider3("Cube pos", obj2.pos, 0.01f);

	GuiManager::EndWindow();
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
			capsuleObj1.pos + Vec3(0,+1.f,0),
			capsuleObj1.pos + Vec3(0,-1.f,0),
			1
		};
		CapsuleCollider c2 =
		{
			capsuleObj2.pos + Vec3(0,+1.f,0),
			capsuleObj2.pos + Vec3(0,-1.f,0),
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
	}

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
	}
	else if (currentCollision == 1)
	{
		GuiManager::DrawSlider3("sphere pos", sphereObj.pos, 0.01f);
		GuiManager::DrawSlider3("triangle pos", triangleObj.pos, 0.01f);
	}
	else if (currentCollision == 2)
	{
		GuiManager::DrawSlider3("ray pos", rayObj.pos, 0.01f);
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

	GuiManager::BeginWindow("Camera Option");
	GuiManager::DrawSlider3("Camera Pos", Camera::current.pos, 0.01f);
	GuiManager::DrawSlider3("Camera Rot", Camera::current.rot, 0.01f);
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

	//ImGui::Text("%f : Lenght", q1.Lenght());

	GuiManager::EndWindow();

}
