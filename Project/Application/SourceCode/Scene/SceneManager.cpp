#include "SceneManager.h"
#include "SceneChanger.h"
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
std::unique_ptr<IScene> SceneManager::sNextScene = nullptr;
bool SceneManager::sIsChanged = false;

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

	// デバッグ時
	ProcessAtDebugBulid([]()
		{
			sCurrentScene = std::make_unique<TitleScene>();
		});

	// リリース時
	ProcessAtReleaseBulid([]()
		{
			sCurrentScene = std::make_unique<LogoScene>();
		});

	FieldDataManager::Load("SkyIsland", "SkyIsland");
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	sCurrentScene->Load();
	sCurrentScene->CreateInstance();
	sCurrentScene->Init();

	mChangeStep = CreateInstance;
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

void SceneManager::DrawDebugGui()
{
	// デバッグ時のみ実行
	ProcessAtDebugBulid([&]()
		{
			sCurrentScene->DrawDebugGui();
		});
}

bool SceneManager::InitNextScene()
{
	// 現在のシーンのアセットをアンロードする
	sCurrentScene->UnLoad();

	// シーン内で使うアセットのロード
	sNextScene->Load();

	// シーン内で使うインスタンス生成
	sNextScene->CreateInstance();

	// シーン初期化
	sNextScene->Init();

	return true;
}

void SceneManager::Draw()
{
	sCurrentScene->Draw();
	SceneChanger::GetInstance()->Draw();
	TransitionManager::GetInstance()->DrawFrontSprite();
}

bool SceneManager::GetisChanged()
{
	return sIsChanged;
}