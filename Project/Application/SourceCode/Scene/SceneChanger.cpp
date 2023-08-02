#include "SceneChanger.h"
#include "SceneManager.h"
using namespace std;

SceneChanger::SceneChanger() :
	mSprite(move(make_unique<Sprite>())),
	mIsSceneChanging(false)
{
	mSprite->SetTexture(TextureManager::GetTexture("White"));
	mSprite->pos = GetWindowHalfSize();
	mSprite->scale = GetWindowSize();
	mSprite->color = Color::black;
}

void SceneChanger::Init()
{
	mIsChange = false;

	mChangeStep = ChangeStep::In;

	mMoveEase.SetEaseTimer(80);
	mMoveEase.SetPowNum(2);
}

void SceneChanger::Update()
{
	if (mIsSceneChanging == false)
	{
		return;
	}

	if (mChangeStep == ChangeStep::In)
	{
		mSprite->color.a = mMoveEase.InOut(0, 255);
		if (mIsEaseTitleBGM == true)
		{
			float volume = mMoveEase.InOut(1, 0);
			SoundManager::SetVolume("TitleBGM", volume);
		}
		else if (mIsEaseGameBGM == true)
		{
			float volume = mMoveEase.InOut(1, 0);
			SoundManager::SetVolume("BattleBGM", volume);
		}

		mMoveEase.Update();
		if (mMoveEase.GetisEnd() == true)
		{
			mChangeStep = ChangeStep::Out;
			mIsChange = true;

			if (mIsEaseTitleBGM == true)
			{
				SoundManager::Stop("TitleBGM");
			}
			else if (mIsEaseGameBGM == true)
			{
				SoundManager::Stop("BattleBGM");
			}

			mMoveEase.Reset();
		}
	}
	else if (mChangeStep == ChangeStep::Out)
	{
		mSprite->color.a = mMoveEase.InOut(255, 0);
		mMoveEase.Update();
		if (mMoveEase.GetisEnd() == true)
		{
			mChangeStep = ChangeStep::End;
			mIsChange = false;
			mIsSceneChanging = false;

			mIsEaseTitleBGM = false;
			mIsEaseGameBGM = false;

			mMoveEase.Reset();
		}
	}

	mSprite->Update();
}

void SceneChanger::Draw()
{
	if (mIsSceneChanging == false) return;

	mSprite->Draw();
}

void SceneChanger::StartSceneChange()
{
	if (mIsSceneChanging == false)
	{
		Init();
		mIsSceneChanging = true;
	}
}

void SceneChanger::SetisEaseTitleBGM(const bool isEaseTitleBGM)
{
	mIsEaseTitleBGM = isEaseTitleBGM;
}

void SceneChanger::SetisEaseGameBGM(const bool isEaseGameBGM)
{
	mIsEaseGameBGM = isEaseGameBGM;
}
