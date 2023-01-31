#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"

std::unique_ptr<IScene> SceneManager::currentScene = nullptr;

SceneManager::SceneManager()
{
	Object3D::isAllLighting = true;
	TextureManager::CreateTexture(Color::white, "White");

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
}

void SceneManager::DrawRenderTexture()
{
	RenderBase::GetInstance()->SetObject3DDrawCommand();
	currentScene->DrawRenderTexture();
}

void SceneManager::DrawBackSprite()
{
	Sprite::SetBlendMode(BlendMode::Alpha);
	currentScene->DrawBackSprite();
}

void SceneManager::DrawModel()
{
	currentScene->DrawModel();
}

void SceneManager::DrawFrontSprite()
{
	Sprite::SetBlendMode(BlendMode::Alpha);
	currentScene->DrawFrontSprite();

#ifdef _DEBUG
	currentScene->DrawDebugGui();
#endif
}