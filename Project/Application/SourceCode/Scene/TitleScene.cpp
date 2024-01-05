#include "TitleScene.h"
#include "GameScene.h"
#include "TransitionManager.h"
#include "MovieEvent.h"
#include "EffectManager.h"
#include "LoadManager.h"

void TitleScene::Load()
{
	LoadManager::GetInstance()->TitleSceneLoad();
}

void TitleScene::UnLoad()
{
	LoadManager::GetInstance()->TitleSceneUnLoad();
}

void TitleScene::CreateInstance()
{
	mTitleUI = std::make_unique<TitleUI>();
	mPostEffectManager = std::make_unique<PostEffectManager>();
}

void TitleScene::Init()
{
	Camera::current.pos = { 0,0,0 };
	Camera::current.rot = { 0,0,0 };

	mPostEffectManager->Init();
	mTitleUI->Init();

	SoundManager::Play("TitleBGM", true);
	SoundManager::SetVolume("TitleBGM", 0);
	mBgmVolume = 0;

	MovieEvent::SetisPlayOnce(false);

	EffectManager::GetInstance()->Init();
	EffectManager::GetInstance()->
		GenerateLogoExplosionEffect(Vec3(0.f, 0.5f, 9.f), Vec3(0, 0, 0), Vec3::one * 0.01f);

	mTimer.SetLimitTimer(360);
}

void TitleScene::Update()
{
	auto currentTransition = TransitionManager::GetInstance()->GetCurrentTransition();
	if (currentTransition == nullptr)
	{
		SoundManager::SetVolume("TitleBGM", mBgmVolume);
		mBgmVolume += 0.01f;
		mBgmVolume = Min<float>(mBgmVolume, 1.f);
	}

	mTimer.Update();
	if (mTimer == true)
	{
		mTimer.Reset();
	}

	Vec2 smoothClamp = mPostEffectManager->GetEffectBloom()->GetHighLumiClmap();
	smoothClamp.x -= sinf(Radian((float)mTimer.GetTimer())) * 0.005f;
	mPostEffectManager->GetEffectBloom()->SetHighLumiClmap(smoothClamp);

	mPostEffectManager->Update();
	mTitleUI->Update();
	EffectManager::GetInstance()->Update();

	// シーン切り替えの処理
	SceneChangeUpdate();
}

void TitleScene::ExecuteCS()
{
	EffectManager::GetInstance()->ExecuteCS();
}

void TitleScene::DrawPass()
{
	mPostEffectManager->DrawBloomPass(
		[this]()
		{
			EffectManager::GetInstance()->DrawEffect();
		},
		[this]()
		{
			EffectManager::GetInstance()->DrawEffect();
		});
}

void TitleScene::Draw()
{
	mPostEffectManager->DrawPostEffect(PostEffectType::Bloom);
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
			EffectManager::GetInstance()->ExplosionLogoExplosionEffect();

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