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

	obj.model = ModelManager::GetModel("Block1");

	spr1.texture = TextureManager::GetTexture("pic");

	renderTex = TextureManager::GetRenderTexture("Test");
	spr2.texture = renderTex;
	spr2.pos = GetWindowHalfSize();
	spr2.graphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture");
}
void TestScene::Update()
{
	Camera::DebugCameraUpdate();

	if (Pad::GetButton(PadCode::ButtonA))
	{
		obj.scale += 0.005f;
	}
	else
	{
		//obj.scale -= 0.005f;
	}

	if (Pad::GetButtonDown(PadCode::ButtonA))
	{
		obj.scale.x = 2.f;
	}

	if (Pad::GetButtonUp(PadCode::ButtonA))
	{
		obj.scale.y = 2.f;
	}

	obj.pos.x = Pad::GetStick(PadCode::LeftStick).x * 0.001f;
	obj.pos.y = -Pad::GetStick(PadCode::LeftStick).y * 0.001f;

	obj.scale.x = Clamp(obj.scale.x, 1, 2);
	obj.scale.y = Clamp(obj.scale.y, 1, 2);
	obj.scale.z = Clamp(obj.scale.z, 1, 2);

	obj.Update();
	spr1.Update();
	spr2.Update();
}

void TestScene::DrawRenderTexture()
{
	renderTex->PreDrawScene();
	obj.Draw();
	renderTex->PostDrawScene();
}
void TestScene::DrawBackSprite()
{
	//spr1.Draw();
	spr2.Draw();
}
void TestScene::DrawModel()
{
	//obj.Draw();
}
void TestScene::DrawFrontSprite()
{

}
void TestScene::DrawDebugGui()
{
}