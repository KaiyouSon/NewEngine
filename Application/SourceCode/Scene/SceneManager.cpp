#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "LogoScene.h"

std::unique_ptr<IScene> SceneManager::currentScene = nullptr;

SceneManager::SceneManager()
{
	Object3D::isAllLighting = true;

	currentScene = std::move(std::make_unique<TitleScene>());
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	currentScene->Init();
}

void SceneManager::Update()
{
	currentScene->Update();
	Camera::current.Update();
}

void SceneManager::DrawRenderTexture()
{
	RenderBase::GetInstance()->SetObject3DDrawCommand();
	currentScene->DrawRenderTexture();
}

void SceneManager::DrawBackSprite()
{
	currentScene->DrawBackSprite();
}

void SceneManager::DrawModel()
{
	currentScene->DrawModel();
}

void SceneManager::DrawFrontSprite()
{
	currentScene->DrawFrontSprite();

#ifdef _DEBUG
	currentScene->DrawDebugGui();
#endif
}