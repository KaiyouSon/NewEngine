#include "SceneManager.h"
#include "TestScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "LogoScene.h"
#include "Bloom.h"
#include "GaussianBlur.h"
#include "Glare.h"
#include "DOF.h"

std::unique_ptr<IScene> SceneManager::currentScene = nullptr;

SceneManager::SceneManager()
{
	Object3D::isAllLighting = true;

	Bloom::CreateGraphicsPipeline();
	GaussianBlur::CreateGraphicsPipeline();
	Glare::CreateGraphicsPipeline();
	DOF::CreateGraphicsPipeline();
	currentScene = std::move(std::make_unique<TestScene>());
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

void SceneManager::RenderTextureSetting()
{
	currentScene->RenderTextureSetting();
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

void SceneManager::DrawRenderTexture()
{
	currentScene->DrawRenderTexture();
}