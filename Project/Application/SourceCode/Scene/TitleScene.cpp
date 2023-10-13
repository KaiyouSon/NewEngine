#include "TitleScene.h"
#include "GameScene.h"
#include "TransitionManager.h"
#include "MovieEvent.h"

void TitleScene::Load()
{
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

	MovieEvent::SetisPlayOnce(false);
}

void TitleScene::Update()
{
	SoundManager::SetVolume("TitleBGM", mBgmVolume);
	mBgmVolume += 0.01f;
	mBgmVolume = Min<float>(mBgmVolume, 1.f);

	mTitleUI->Update();

	// シーン切り替えの処理
	SceneChangeUpdate();
}

void TitleScene::DrawPass()
{
}

void TitleScene::Draw()
{
	mTitleUI->DrawFrontSprite();
}

void TitleScene::DrawDebugGui()
{
}

// シーン切り替えの処理
void TitleScene::SceneChangeUpdate()
{
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

		// トランジションがnullかつボタン押してないなら
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