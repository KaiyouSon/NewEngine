#include "TitleScene.h"
#include "GameScene.h"
#include "SceneChanger.h"

void TitleScene::Load()
{
	TextureManager::LoadTexture("Title/TitleLogo.png", "TitleLogo");
	TextureManager::LoadTexture("Title/GameTitle.png", "GameTitle");
	TextureManager::LoadTexture("Title/PressButton.png", "PressButton");
	TextureManager::LoadTexture("Title/PressButtonBack.png", "PressButtonBack");

	TextureManager::ExcuteComandList();
}

void TitleScene::UnLoad()
{
	TextureManager::UnLoadTexture("TitleLogo");
	TextureManager::UnLoadTexture("GameTitle");
	TextureManager::UnLoadTexture("PressButton");
	TextureManager::UnLoadTexture("PressButtonBack");
}

void TitleScene::Init()
{
	Camera::current.pos = { 0,0,-10 };
	Camera::current.rot = { 0,0,0 };

	mTitleUI = std::make_unique<TitleUI>();
	mTitleUI->Init();

	SoundManager::Play("TitleBGM", true);
	SoundManager::SetVolume("TitleBGM", 0);
	mBgmVolume = 0;
}

void TitleScene::Update()
{
	SoundManager::SetVolume("TitleBGM", mBgmVolume);
	mBgmVolume += 0.01f;
	mBgmVolume = Min<float>(mBgmVolume, 1.f);

	mTitleUI->Update();

	if (Pad::GetAnyButtonDown())
	{
		if (mTitleUI->GetisAfterImage() == false)
		{
			SoundManager::Play("SelectSE");
			mTitleUI->SetisAfterImage(true);
		}
	}

	if (mTitleUI->GetisEnd() == true)
	{
		if (SceneChanger::GetInstance()->GetisSceneChanging() == false)
		{
			SceneChanger::GetInstance()->StartSceneChange();
			SceneChanger::GetInstance()->SetisEaseTitleBGM(true);
		}

		if (SceneChanger::GetInstance()->GetisChange() == true)
		{
			SceneManager::ChangeScene<GameScene>();
			SceneChanger::GetInstance()->SetisChange(false);

		}
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
	mTitleUI->DrawFrontSprite();
}

void TitleScene::DrawDebugGui()
{
}
