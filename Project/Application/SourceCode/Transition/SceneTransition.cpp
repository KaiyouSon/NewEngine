#include "SceneTransition.h"

SceneTransition::SceneTransition() :
	mTransition(std::make_unique<Sprite>()),
	mLoadSprite(std::make_unique<Sprite>())
{

	// 種類設定
	mType = TransitionType::Scene;
}

void SceneTransition::Generate()
{
	// 初期設定
	mTransition->SetTexture(TextureManager::GetTexture("White"));
	mTransition->pos = GetWindowHalfSize();
	mTransition->scale = GetWindowSize();
	mTransition->color = Color::black;

	mLoadSprite->SetTexture(TextureManager::GetTexture("TitleLogo"));
	mLoadSprite->pos = GetWindowSize() - 64.f;
	mLoadSprite->scale = 0.1f;

	mEase.SetEaseTimer(80);
	mEase.SetPowNum(2);
	mEase.Reset();

	mAlphaTimer.SetLimitTimer(180);

	mStep = TransitionStep::None;
}

void SceneTransition::Update()
{
	switch (mStep)
	{
	case TransitionStep::In: // 遷移がインの時
	{
		mTransition->color.a = mEase.InOut(0, 255);
		mLoadSprite->color.a = mEase.InOut(0, 255);

		if (SoundManager::GetIsPlaying("TitleBGM") == true)
		{
			float volume = mEase.InOut(1, 0);
			SoundManager::SetVolume("TitleBGM", volume);
		}

		if (SoundManager::GetIsPlaying("BattleBGM") == true)
		{
			float volume = mEase.InOut(1, 0);
			SoundManager::SetVolume("BattleBGM", volume);
		}

		mEase.Update();
		if (mEase.GetisEnd() == true)
		{
			mStep = TransitionStep::Progress;

			// アウトの時ちょっと早く感じたので時間を伸ばした
			mEase.SetEaseTimer(100);
			mEase.Reset();
		}
	}
	break;

	case TransitionStep::Progress:
	{
		mLoadSprite->rot += Radian(10);

		if (SoundManager::GetIsPlaying("TitleBGM") == true)
		{
			SoundManager::Stop("TitleBGM");
		}

		if (SoundManager::GetIsPlaying("BattleBGM") == true)
		{
			SoundManager::Stop("BattleBGM");
		}

		//mLoadSprite->color.a = cosf(Radian((float)mAlphaTimer.GetTimer())) * 255.f;

		//mAlphaTimer.Update(10);
		//if (mAlphaTimer == true)
		//{
		//	mAlphaTimer.Reset();
		//}
	}
	break;

	case TransitionStep::Out: // 遷移がアウトの時
	{
		SceneManager::SetChangeStepToCreateInstance();

		mTransition->color.a = mEase.InOut(255, 0);
		mLoadSprite->color.a = mEase.InOut(255, 0);

		mEase.Update();
		if (mEase.GetisEnd() == true)
		{
			mStep = TransitionStep::End;
			mEase.Reset();
		}
	}
	break;
	}

	mLoadSprite->Update();
	mTransition->Update();
}

void SceneTransition::DrawFrontSprite()
{
	if (mStep == TransitionStep::None)
	{
		return;
	}

	mTransition->Draw();
	mLoadSprite->Draw();
}
