#include "GameScene.h"

GameScene::GameScene() :
	player(std::make_unique<Player>()),
	uiManager(std::make_unique<UIManager>())
{
}
GameScene::~GameScene()
{
}

void GameScene::Init()
{
	Camera::current.pos = { 0,1,-15 };
	Camera::current.rot = { Radian(0),0,0 };

	player->Init();

	uiManager->SetPlayer(player.get());
	uiManager->Init();
}
void GameScene::Update()
{
	player->Update();
	uiManager->Update();

	Camera::DebugCameraUpdate();
}

void GameScene::RenderTextureSetting()
{
}

void GameScene::DrawRenderTexture()
{
}
void GameScene::DrawBackSprite()
{
}
void GameScene::DrawModel()
{
	player->DrawModel();
}
void GameScene::DrawFrontSprite()
{
	uiManager->DrawFrontSprite();
}
void GameScene::DrawDebugGui()
{
	player->DrawDebugGui();
}