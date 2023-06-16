#include "TitleScene.h"

void TitleScene::Init()
{
	Camera::current.pos = { 0,10,-10 };
	Camera::current.rot = { Radian(45),0,0 };
}

void TitleScene::Update()
{
	Camera::DebugCameraUpdate();
}

void TitleScene::RenderTextureSetting()
{
}

void TitleScene::DrawRenderTexture()
{
}

void TitleScene::DrawBackSprite()
{
}

void TitleScene::DrawModel()
{
}

void TitleScene::DrawFrontSprite()
{
}

void TitleScene::DrawDebugGui()
{
}
