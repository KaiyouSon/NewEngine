#include "TitleScene.h"
#include "GameScene.h"

void TitleScene::Init()
{
	Camera::current.pos = { 0,10,-10 };
	Camera::current.rot = { Radian(45),0,0 };

	titleUI_ = std::make_unique<TitleUI>();
	titleUI_->Init();
}

void TitleScene::Update()
{
	titleUI_->Update();

	if (Pad::GetAnyButtonDown())
	{
		SceneManager::ChangeScene<GameScene>();
	}
}

void TitleScene::RenderTextureSetting()
{
}

void TitleScene::DrawRenderTexture()
{
}

void TitleScene::DrawBackSprite()
{
}

void TitleScene::DrawModel()
{
}

void TitleScene::DrawFrontSprite()
{
	titleUI_->DrawFrontSprite();
}

void TitleScene::DrawDebugGui()
{
}
