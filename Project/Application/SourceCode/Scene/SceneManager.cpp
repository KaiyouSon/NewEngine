#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "LogoScene.h"
#include "FieldDataManager.h"
#include "TransitionManager.h"
#include "CreateManager.h"

std::unique_ptr<IScene> SceneManager::sCurrentScene = nullptr;
std::unique_ptr<IScene> SceneManager::sNextScene = nullptr;

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

	mChangeStep = None;

	mIsReturn = false;
}

void SceneManager::Update()
{
	if (mChangeStep == Changed)
	{
		mChangeStep = None;
	}

	Camera::current.Update();
	sCurrentScene->Update();
	TransitionManager::GetInstance()->Update();
}

void SceneManager::DrawPass()
{
	if (mChangeStep != None)
	{
		return;
	}

	sCurrentScene->DrawPass();
}

void SceneManager::DrawDebugGui()
{
	// デバッグビルドのも実行
	ProcessAtDebugBuild([&]()
		{
			if (mChangeStep != None)
			{
				return;
			}

			sCurrentScene->DrawDebugGui();
		});
}

void SceneManager::Draw()
{
	if (mChangeStep == None)
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
	//GetInstance()->mChangeStep = None;
}

