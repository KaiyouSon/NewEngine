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
}
void TestScene::Update()
{
	Camera::DebugCameraUpdate();
}

void TestScene::DrawRenderTexture()
{
}
void TestScene::DrawBackSprite()
{
}
void TestScene::DrawModel()
{
}
void TestScene::DrawFrontSprite()
{
}
void TestScene::DrawDebugGui()
{
}