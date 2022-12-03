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

Object3D obj;

// 画像の読み込み
void Load()
{
	Model test = Model("Cube");
	Texture tex = Texture("pic.png");

	obj.model = test;
	obj.texture = tex;
}
// 初期化処理
void Initialize()
{
	//view->SetPos(Vec3(0, 0, -50));
	//view->SetTarget(Vec3::zero);
	//view->SetUp(Vec3::up);

	//obj.Initialize();

	DebugCamera::GetInstance()->Initialize();
}

// 更新処理
void Update()
{
	obj.Update();

	view->SetPos(debugCamera->GetPos());
	view->SetTarget(debugCamera->GetTarget());
	view->SetUp(debugCamera->GetUp());
	view->Update();
	DebugCamera::GetInstance()->Update();
}

void Draw2D()
{
}

void Draw3D()
{
	obj.Draw();
}

void DrawLine()
{
}

// インスタンスのdelete
void Destroy()
{
}