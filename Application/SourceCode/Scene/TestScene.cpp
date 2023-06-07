#include "TestScene.h"

TestScene::TestScene()
{
}
TestScene::~TestScene()
{
}

void TestScene::Init()
{
	Camera::current.pos = { 0,1,-15 };
	Camera::current.rot = { Radian(0),0,0 };

	obj1.SetModel(ModelManager::GetModel("Block1"));
	obj2.SetModel(ModelManager::GetModel("Block1"));
	obj2.pos_.x = 3;

	spr1.texture = TextureManager::GetTexture("pic");

	renderTex = TextureManager::GetRenderTexture("Test");
	spr2.texture = renderTex;
	spr2.pos = GetWindowHalfSize();
	spr2.graphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture");
}
void TestScene::Update()
{
	Camera::DebugCameraUpdate();

	const float speed = 0.05f;
	obj1.pos_.x += (Key::GetKey(DIK_D) - Key::GetKey(DIK_A)) * speed;
	obj1.pos_.y += (Key::GetKey(DIK_W) - Key::GetKey(DIK_S)) * speed;

	if (Key::GetKeyDown(DIK_SPACE) == true)
	{
		obj2.SetTexture(TextureManager::GetTexture("NumberSheet"));

		//obj2.SetModel(ModelManager::GetModel("Enemy"));
	}

	obj1.Update();
	Transform tf = obj1.GetTransform();

	obj2.Update(&tf);
	spr1.Update();
	spr2.Update();
}

void TestScene::DrawRenderTexture()
{
	renderTex->PreDrawScene();
	obj1.Draw();
	renderTex->PostDrawScene();
}
void TestScene::DrawBackSprite()
{
	//spr1.Draw();
	//spr2.Draw();
}
void TestScene::DrawModel()
{
	obj1.Draw();
	obj2.Draw();
}
void TestScene::DrawFrontSprite()
{

}
void TestScene::DrawDebugGui()
{
}