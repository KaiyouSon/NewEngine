#include "GameScene.h"
#include "SceneChanger.h"
#include "CollisionManager.h"

GameScene::GameScene() :
	player_(std::make_unique<Player>()),
	boss_(std::make_unique<Boss>()),
	uiManager_(std::make_unique<UIManager>()),
	cameraManager_(std::make_unique<CameraManager>()),
	field_(std::make_unique<Field>())
{

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

	field_->Init();

	CollisionManager::GetInstance()->SetPlayer(player_.get());
	CollisionManager::GetInstance()->SetBoss(boss_.get());
	CollisionManager::GetInstance()->SetField(field_.get());

	LightManager::GetInstance()->directionalLight.isActive = true;
	LightManager::GetInstance()->directionalLight.pos = Vec3(1, 1, 0);
}
void GameScene::Update()
{
	player_->PrevUpdate();
	boss_->Update();
	uiManager_->Update();
	CollisionManager::GetInstance()->Update();
	player_->PostUpdate();
	field_->Update();

	cameraManager_->Update();
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
	field_->DrawModel();
}
void GameScene::DrawFrontSprite()
{
	uiManager_->DrawFrontSprite();

}
void GameScene::DrawDebugGui()
{
	GuiManager::BeginWindow("Lighting");
	GuiManager::DrawCheckBox("isActive", &LightManager::GetInstance()->directionalLight.isActive);
	GuiManager::DrawSlider3("dirVec", LightManager::GetInstance()->directionalLight.pos, 0.01f);
	GuiManager::DrawColorEdit("color", LightManager::GetInstance()->directionalLight.color);
	GuiManager::EndWindow();

	//player_->DrawDebugGui();
}