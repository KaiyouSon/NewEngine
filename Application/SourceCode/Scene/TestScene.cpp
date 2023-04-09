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