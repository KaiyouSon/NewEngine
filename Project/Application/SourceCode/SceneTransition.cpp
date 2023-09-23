#include "SceneTransition.h"

SceneTransition::SceneTransition() :
	mTransition(std::make_unique<Sprite>()),
	mLoadSprite(std::make_unique<Sprite>())
{

	// ��ސݒ�
	mType = TransitionType::Scene;
}

void SceneTransition::Generate()
{
	// �����ݒ�
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
	case TransitionStep::In: // �J�ڂ��C���̎�
	{
		mTransition->color.a = mEase.InOut(0, 255);
		mLoadSprite->color.a = mEase.InOut(0, 255);

		mEase.Update();
		if (mEase.GetisEnd() == true)
		{
			mStep = TransitionStep::Progress;

			// �A�E�g�̎�������Ƒ����������̂Ŏ��Ԃ�L�΂���
			mEase.SetEaseTimer(100);
			mEase.Reset();
		}
	}
	break;

	case TransitionStep::Progress:
	{
		mLoadSprite->rot += Radian(10);

		//mLoadSprite->color.a = cosf(Radian((float)mAlphaTimer.GetTimer())) * 255.f;

		//mAlphaTimer.Update(10);
		//if (mAlphaTimer == true)
		//{
		//	mAlphaTimer.Reset();
		//}
	}
	break;

	case TransitionStep::Out: // �J�ڂ��A�E�g�̎�
	{
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
