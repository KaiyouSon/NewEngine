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
#include "Cloud.h"

std::unique_ptr<IScene> SceneManager::sCurrentScene = nullptr;

SceneManager::SceneManager()
{
	Object3D::isAllLighting = true;
	Cloud::CreateGraphicsPipeline();

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
	sCurrentScene->Update();
	Camera::current.Update();
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