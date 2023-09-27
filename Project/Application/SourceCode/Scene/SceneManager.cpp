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
#include "EffectManager.h"

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
	EffectManager::CreateGraphicsPipeline();

	Bloom::CreateGraphicsPipeline();

	// デバッグ時
	ProcessAtDebugBulid([]()
		{
			sCurrentScene = std::make_unique<GameScene>();
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
	mTestTimer.SetLimitTimer(300);

	mIsReturn = false;
}

void SceneManager::Update()
{
	//mChangeStep = CreateInstance;

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

void SceneManager::Draw()
{
	//if (mIsReturn == true)
	//{
	//	return;
	//}

	if (SceneManager::GetisLoading() == false &&
		SceneManager::GetisChanged() == false)
	{
		sCurrentScene->Draw();
	}

	SceneChanger::GetInstance()->Draw();
	TransitionManager::GetInstance()->DrawFrontSprite();
}

bool SceneManager::GetisLoading()
{
	return GetInstance()->mChangeStep == Loading;
}

bool SceneManager::GetisChanged()
{
	return GetInstance()->mChangeStep == Changed;
}

void SceneManager::SetChangeStepToCreateInstance()
{
	GetInstance()->mIsReturn = false;
	GetInstance()->mChangeStep = CreateInstance;
}
