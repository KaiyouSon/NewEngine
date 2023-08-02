#include "ResultUI.h"

bool ResultUI::sIsEnd = false;

ResultUI::ResultUI() :
	mBack(std::make_unique<Sprite>()),
	mText(std::make_unique<Sprite>()),
	mTextAfterImage(std::make_unique<Sprite>())
{
	mBack->SetTexture(TextureManager::GetTexture("ResultBack"));
	mText->SetTexture(TextureManager::GetTexture("YouDiedStr"));
	mTextAfterImage->SetTexture(TextureManager::GetTexture("EnemyFelledStr"));
}
void ResultUI::Init()
{
	mBack->scale.x = 15.f;
	mBack->color.a = 0.f;
	mText->color.a = 0.f;

	mParent.pos = GetWindowHalfSize();
	mParent.Update();

	mTimer.SetLimitTimer(180);

	mIsAfterImage = false;
	mTextAfterImage->color.a = 0.f;

	sIsEnd = false;
}
void ResultUI::Update()
{
	if (mIsActive == true)
	{
		switch (mResultType)
		{
		case ResultType::EnemyFelledStr:
			EnemyFelledUpdate();
			break;

		case ResultType::YouDiedStr:
			YouDiedUpdate();
			break;

		default:
			break;
		}
	}

	mBack->Update(&mParent);
	mText->Update(&mParent);
	mTextAfterImage->Update(&mParent);
}
void ResultUI::DrawFrontSprite()
{
	mBack->Draw();

	switch (mResultType)
	{
	case ResultType::EnemyFelledStr:
		EnemyFelledDraw();
		break;

	case ResultType::YouDiedStr:
		YouDiedDraw();
		break;

	default:
		break;
	}

}

void ResultUI::EnemyFelledUpdate()
{
	if (sIsEnd == true)
	{
		return;
	}
	mWaitTimer.SetLimitTimer(120);

	if (mIsWait == true)
	{
		mWaitTimer.Update();
		if (mWaitTimer == true)
		{
			mWaitTimer.Reset();
			mIsWait = false;
		}
	}
	else
	{
		mBack->color.a = sinf(Radian((float)mTimer.GetTimer())) * 245.f;
		mText->color.a = sinf(Radian((float)mTimer.GetTimer())) * 245.f;

		if (mIsAfterImage == false)
		{
			mText->scale += 0.001f;
		}

		mTimer.Update();
		// 90“x‚ÌŽž
		if (mTimer.GetTimer() == 90)
		{
			if (mIsAfterImage == false)
			{
				mIsAfterImage = true;
				mTextAfterImage->scale = mText->scale;
			}

			if (mIsWait == false)
			{
				mIsWait = true;
			}
		}
	}

	if (mIsAfterImage == true)
	{
		mTextAfterImage->scale += 0.0005f;
		mTextAfterImage->color.a = sinf(Radian((float)mTimer.GetTimer())) * 55.f;
	}

	if (mTimer.GetisTimeOut() == true)
	{
		mBack->color.a = 0;
		mText->color.a = 0;
		mTextAfterImage->color.a = 0;
		mText->scale = 1;

		mTimer.Reset();
		mIsActive = false;
		mIsAfterImage = false;
		sIsEnd = true;
	}
}
void ResultUI::EnemyFelledDraw()
{
	mTextAfterImage->Draw();
	mText->Draw();
}

void ResultUI::YouDiedUpdate()
{
	if (sIsEnd == true)
	{
		return;
	}

	mWaitTimer.SetLimitTimer(60);

	if (mIsWait == true)
	{
		mWaitTimer.Update();
		if (mWaitTimer == true)
		{
			mWaitTimer.Reset();
			mIsWait = false;
		}
	}
	else
	{
		mBack->color.a = sinf(Radian((float)mTimer.GetTimer())) * 245.f;
		mText->color.a = sinf(Radian((float)mTimer.GetTimer())) * 245.f;
		mText->scale += 0.001f;

		mTimer.Update();
		// 90“x‚ÌŽž
		if (mTimer.GetTimer() == 90)
		{
			if (mIsWait == false)
			{
				mIsWait = true;
			}
		}
	}

	if (mTimer.GetisTimeOut() == true)
	{
		mBack->color.a = 0;
		mText->color.a = 0;
		mText->scale = 1;

		mTimer.Reset();
		mIsActive = false;
		sIsEnd = true;
	}
}
void ResultUI::YouDiedDraw()
{
	mText->Draw();
}

void ResultUI::SetisActive(const bool isActive)
{
	mIsActive = isActive;
}
void ResultUI::SetResultType(const ResultType resultType)
{
	mResultType = resultType;

	switch (mResultType)
	{
	case ResultType::EnemyFelledStr:
		mText->SetTexture(TextureManager::GetTexture("EnemyFelledStr"));
		break;

	case ResultType::YouDiedStr:
		mText->SetTexture(TextureManager::GetTexture("YouDiedStr"));
		break;

	default:
		break;
	}
}

bool ResultUI::GetisEnd()
{
	return sIsEnd;
}
