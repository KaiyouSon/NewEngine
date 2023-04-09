#include "GameScene.h"

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
}
void GameScene::Update()
{
	Camera::DebugCameraUpdate();
}

void GameScene::DrawRenderTexture()
{
}
void GameScene::DrawBackSprite()
{
}
void GameScene::DrawModel()
{
}
void GameScene::DrawFrontSprite()
{
}
void GameScene::DrawDebugGui()
{
}