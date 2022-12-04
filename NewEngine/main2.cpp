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

	for (int i = 0; i < 10; i++)
	{
		obj[i].model = test;
		obj[i].texture = tex;
	}

	spr.texture = tex;
}

void TestScene::Init()
{
	DebugCamera::GetInstance()->Initialize();
}

void TestScene::Update()
{
	for (int i = 0; i < 10; i++)
	{
		const float offsetAngle = (360 / 10) * i;
		obj[i].pos = { sinf(mathUtil->Radian(offsetAngle)) * 30, 0, cosf(mathUtil->Radian(offsetAngle)) * 30 };
		obj[i].rot.y = offsetAngle;
		obj[i].Update();
	}
	spr.Update();

	Camera::current.Update();
	DebugCamera::GetInstance()->Update();
}

void TestScene::DrawFrontSprite()
{
}

void TestScene::DrawBackSprite()
{
}

void TestScene::DrawModel()
{
	for (int i = 0; i < 10; i++)
	{
		obj[i].Draw();
	}
}
