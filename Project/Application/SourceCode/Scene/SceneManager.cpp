#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "LogoScene.h"
#include "FieldDataManager.h"
#include "TransitionManager.h"
#include "CreateManager.h"

std::unique_ptr<IScene> SceneManager::sCurrentScene = nullptr;
std::unique_ptr<IScene> SceneManager::sNextScene = nullptr;
bool SceneManager::sIsChanged = false;

SceneManager::SceneManager()
{
	Object3D::isAllLighting = true;

	CreateManager::GetInstance()->Create();

	// デバッグビルドのも実行
	ProcessAtDebugBuild([]()
		{
			sCurrentScene = std::make_unique<GameScene>();
		});

	// リリースビルドのも実行
	ProcessAtReleaseBuild([]()
		{
			sCurrentScene = std::make_unique<LogoScene>();
		});
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

	mIsReturn = false;
}

void SceneManager::Update()
{
	Camera::current.Update();
	sCurrentScene->Update();
	TransitionManager::GetInstance()->Update();
}

void SceneManager::DrawPass()
{
	sCurrentScene->DrawPass();
}

void SceneManager::DrawDebugGui()
{
	// デバッグビルドのも実行
	ProcessAtDebugBuild([&]()
		{
			sCurrentScene->DrawDebugGui();
		});
}

void SceneManager::Draw()
{
	if (SceneManager::GetisLoading() == false &&
		SceneManager::GetisChanged() == false)
	{
		sCurrentScene->Draw();
	}

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

