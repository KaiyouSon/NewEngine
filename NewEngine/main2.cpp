#include "main2.h"
#include "Util.h"
#include "MathUtil.h"
#include "InputManager.h"
#include "DrawManager.h"
#include "Sound.h"
#include <memory>
#include "Collision.h"
#include "Model.h"
#include "Texture.h"
using namespace std;

void TestScene::Load()
{
	Model test = Model("Cube");
	Texture tex = Texture("pic.png");
	obj.model = test;
	obj.texture = tex;

	spr.texture = tex;
}

void TestScene::Init()
{
	//view->SetPos(Vec3(0, 0, -50));
	//view->SetTarget(Vec3::zero);
	//view->SetUp(Vec3::up);


	DebugCamera::GetInstance()->Initialize();
}

void TestScene::Update()
{
	obj.Update();
	spr.Update();

	view->SetPos(debugCamera->GetPos());
	view->SetTarget(debugCamera->GetTarget());
	view->SetUp(debugCamera->GetUp());
	view->Update();
	DebugCamera::GetInstance()->Update();
}

void TestScene::DrawFrontSprite()
{
}

void TestScene::DrawBackSprite()
{
	spr.Draw();
}

void TestScene::DrawModel()
{
	obj.Draw();
}
