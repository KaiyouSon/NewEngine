#include "SceneManager.h"
#include "SceneChanger.h"
#include "TestScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "LogoScene.h"
#include "Bloom.h"
#include "GaussianBlur.h"
#include "Glare.h"
#include "DOF.h"
#include "RadialBlur.h"
#include "Vignette.h"
#include "ShadowMap.h"
#include "Cloud.h"
#include "RespawnPoint.h"
#include "RespawnPointUI.h"
#include "Tree.h"
#include "Grass.h"
#include "Skydome.h"
#include "FieldDataManager.h"
#include "TransitionManager.h"

std::unique_ptr<IScene> SceneManager::sCurrentScene = nullptr;

SceneManager::SceneManager()
{
	Object3D::isAllLighting = true;
	ShadowMap::CreateGraphicsPipeline();
	ShadowObj::CreateGraphicsPipeline();
	Cloud::CreateGraphicsPipeline();
	RespawnPoint::CreateGraphicsPipeline();
	RespawnPointUI::CreateGraphicsPipeline();
	Tree::CreateGraphicsPipeline();
	Grass::CreateGraphicsPipeline();
	Skydome::CreateGraphicsPipeline();

	FieldDataManager::Load("SkyIsland", "SkyIsland");


	//Bloom::CreateGraphicsPipeline();
	//GaussianBlur::CreateGraphicsPipeline();
	//Glare::CreateGraphicsPipeline();
	//DOF::CreateGraphicsPipeline();
	//RadialBlur::CreateGraphicsPipeline();
	//Vignette::CreateGraphicsPipeline();
	sCurrentScene = std::make_unique<GameScene>();
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	sCurrentScene->Init();
}

void SceneManager::Update()
{
	Camera::current.Update();
	sCurrentScene->Update();
	TransitionManager::GetInstance()->Update();

	SceneChanger::GetInstance()->Update();
}

void SceneManager::RenderTextureSetting()
{
	sCurrentScene->RenderTextureSetting();
}

void SceneManager::DrawBackSprite()
{
	sCurrentScene->DrawBackSprite();
}

void SceneManager::DrawModel()
{
	sCurrentScene->DrawModel();
}

void SceneManager::DrawFrontSprite()
{
	sCurrentScene->DrawFrontSprite();
	SceneChanger::GetInstance()->Draw();
	TransitionManager::GetInstance()->DrawFrontSprite();

	// デバッグ時のみ実行
	ProcessAtDebugBulid([&]()
		{
			sCurrentScene->DrawDebugGui();
		});
}

void SceneManager::DrawRenderTexture()
{
	sCurrentScene->DrawRenderTexture();
}