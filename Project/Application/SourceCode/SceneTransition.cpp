#include "SceneTransition.h"

SceneTransition::SceneTransition()
{
	mTransition = std::make_unique<Sprite>();

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

	mEase.SetEaseTimer(80);
	mEase.SetPowNum(2);
	mEase.Reset();

	mStep = TransitionStep::None;
}

void SceneTransition::Update()
{
	switch (mStep)
	{
	case TransitionStep::In: // 遷移がインの時
	{
		mTransition->color.a = mEase.InOut(0, 255);

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

	case TransitionStep::Out: // 遷移がアウトの時
	{
		mTransition->color.a = mEase.InOut(255, 0);

		mEase.Update();
		if (mEase.GetisEnd() == true)
		{
			mStep = TransitionStep::End;
			mEase.Reset();
		}
	}
	break;
	}

	mTransition->Update();
}

void SceneTransition::DrawFrontSprite()
{
	if (mStep == TransitionStep::None)
	{
		return;
	}

	mTransition->Draw();
}
