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

std::unique_ptr<IScene> SceneManager::currentScene = nullptr;

SceneManager::SceneManager()
{
	Object3D::isAllLighting = true;

	Bloom::CreateGraphicsPipeline();
	GaussianBlur::CreateGraphicsPipeline();
	Glare::CreateGraphicsPipeline();
	DOF::CreateGraphicsPipeline();
	RadialBlur::CreateGraphicsPipeline();
	Vignette::CreateGraphicsPipeline();
	currentScene = std::move(std::make_unique<GameScene>());
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
	SceneChanger::GetInstance()->Update();
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
	SceneChanger::GetInstance()->Draw();

#ifdef _DEBUG
#endif
	currentScene->DrawDebugGui();
}

void SceneManager::DrawRenderTexture()
{
	currentScene->DrawRenderTexture();
}