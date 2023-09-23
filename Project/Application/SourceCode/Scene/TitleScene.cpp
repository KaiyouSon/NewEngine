#include "TitleScene.h"
#include "GameScene.h"
#include "SceneChanger.h"
#include "TransitionManager.h"

void TitleScene::Load()
{
	// シーン遷移に使うからUnLoadしない
	TextureManager::LoadTexture("Title/TitleLogo.png", "TitleLogo");

	TextureManager::LoadTexture("Title/GameTitle.png", "GameTitle");
	TextureManager::LoadTexture("Title/PressButton.png", "PressButton");
	TextureManager::LoadTexture("Title/PressButtonBack.png", "PressButtonBack");

	TextureManager::ExcuteComandList();
}

void TitleScene::UnLoad()
{
	TextureManager::UnLoadTexture("GameTitle");
	TextureManager::UnLoadTexture("PressButton");
	TextureManager::UnLoadTexture("PressButtonBack");
}

void TitleScene::CreateInstance()
{
	mTitleUI = std::make_unique<TitleUI>();
}

void TitleScene::Init()
{
	Camera::current.pos = { 0,0,-10 };
	Camera::current.rot = { 0,0,0 };

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

	if (Key::GetKeyDown(DIK_S))
	{
		TransitionManager::GetInstance()->Start(TransitionType::Scene);
	}
	if (Key::GetKeyDown(DIK_E))
	{
		TransitionManager::GetInstance()->End();
	}

	if (Pad::GetAnyButtonDown())
	{
		if (mTitleUI->GetisAfterImage() == false)
		{
			SoundManager::Play("SelectSE");
			mTitleUI->SetisAfterImage(true);

			if (mIsPush == false)
			{
				mIsPush = true;
			}
		}
	}

	if (mTitleUI->GetisEnd() == true)
	{
		auto currentTransition = TransitionManager::GetInstance()->GetCurrentTransition();

		// トランジションがnullかつボタン押してない場合
		if (currentTransition == nullptr)
		{
			if (mIsPush == true)
			{
				TransitionManager::GetInstance()->Start(TransitionType::Scene);
				mIsPush = false;
			}
		}
		else
		{
			if (currentTransition->GetType() == TransitionType::Scene &&
				currentTransition->GetStep() == TransitionStep::Progress)
			{
				SceneManager::ChangeScene<GameScene>();
				if (SceneManager::GetisChanged() == true)
				{
					TransitionManager::GetInstance()->End();
				}
			}
		}
	}
}

void TitleScene::RenderTextureSetting()
{
}

void TitleScene::Draw()
{
	if (SceneManager::GetisLoading() == true ||
		SceneManager::GetisChanged() == true)
	{
		return;
	}

	mTitleUI->DrawFrontSprite();
}

void TitleScene::DrawDebugGui()
{
}
