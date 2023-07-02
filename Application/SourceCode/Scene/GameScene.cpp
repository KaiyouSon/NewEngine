#include "GameScene.h"

GameScene::GameScene() :
	player_(std::make_unique<Player>()),
	boss_(std::make_unique<Boss>()),
	uiManager_(std::make_unique<UIManager>()),
	cameraManager_(std::make_unique<CameraManager>()),
	collisionManager_(std::make_unique<CollisionManager>()),
	ground_(std::make_unique<Object3D>())
{
	ground_->SetModel(ModelManager::GetModel("Ground"));
	ground_->scale = 1000.f;
	ground_->tiling = 100;
}
GameScene::~GameScene()
{
}

void GameScene::Init()
{
	Camera::current.pos = { 0,1,-15 };
	Camera::current.rot = { Radian(0),0,0 };

	player_->Init();

	boss_->Init();

	uiManager_->SetPlayer(player_.get());
	uiManager_->SetBoss(boss_.get());
	uiManager_->Init();

	cameraManager_->SetPlayer(player_.get());
	cameraManager_->Init();

	collisionManager_->SetPlayer(player_.get());
	collisionManager_->SetBoss(boss_.get());
}
void GameScene::Update()
{
	player_->Update();
	boss_->Update();
	uiManager_->Update();
	ground_->Update();

	cameraManager_->Update();
	collisionManager_->Update();

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
	player_->DrawModel();
	boss_->DrawModel();
	ground_->Draw();
}
void GameScene::DrawFrontSprite()
{
	uiManager_->DrawFrontSprite();
}
void GameScene::DrawDebugGui()
{
	player_->DrawDebugGui();
}