#include "GameScene.h"

GameScene::GameScene() :
	player(std::make_unique<Player>()),
	uiManager(std::make_unique<UIManager>()),
	cameraManager(std::make_unique<CameraManager>()),
	ground(std::make_unique<Object3D>())
{
	ground->SetModel(ModelManager::GetModel("Ground"));
	ground->scale = 1000.f;
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

	cameraManager->SetPlayer(player.get());
	cameraManager->Init();
}
void GameScene::Update()
{
	player->Update();
	uiManager->Update();
	ground->Update();

	cameraManager->Update();
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
	ground->Draw();
}
void GameScene::DrawFrontSprite()
{
	uiManager->DrawFrontSprite();
}
void GameScene::DrawDebugGui()
{
	player->DrawDebugGui();
}