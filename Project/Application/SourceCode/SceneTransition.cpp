#include "SceneTransition.h"

SceneTransition::SceneTransition()
{
	mTransition = std::make_unique<Sprite>();

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

	mEase.SetEaseTimer(80);
	mEase.SetPowNum(2);
	mEase.Reset();

	mStep = TransitionStep::None;
}

void SceneTransition::Update()
{
	switch (mStep)
	{
	case TransitionStep::In: // �J�ڂ��C���̎�
	{
		mTransition->color.a = mEase.InOut(0, 255);

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

	case TransitionStep::Out: // �J�ڂ��A�E�g�̎�
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
