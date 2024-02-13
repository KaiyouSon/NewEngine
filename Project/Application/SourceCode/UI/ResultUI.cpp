#include "ResultUI.h"

bool ResultUI::sIsEnd = false;

ResultUI::ResultUI() :
	mBack(std::make_unique<Sprite>()),
	mText(std::make_unique<Sprite>()),
	mTextAfterImage(std::make_unique<Sprite>())
{
	mBack->SetTexture("ResultBack");
	mText->SetTexture("YouDiedStr");
	mTextAfterImage->SetTexture("EnemyFelledStr");
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
		case ResultType::EnemyFelledText:
			EnemyFelledUpdate();
			break;

		case ResultType::YouDiedText:
			YouDiedUpdate();
			break;

		default:
			break;
		}
	}

	mBack->SetParent(&mParent);
	mText->SetParent(&mParent);
	mTextAfterImage->SetParent(&mParent);

	mBack->Update();
	mText->Update();
	mTextAfterImage->Update();
}
void ResultUI::Draw()
{
	mBack->Draw("FrontSprite");

	switch (mResultType)
	{
	case ResultType::EnemyFelledText:
		EnemyFelledDraw();
		break;

	case ResultType::YouDiedText:
		YouDiedDraw();
		break;

	default:
		break;
	}

}

// 敵を倒した時の処理
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
		// 90度の時
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
	mTextAfterImage->Draw("FrontSprite");
	mText->Draw("FrontSprite");
}

// プレイヤーが死んだ時の処理
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
		// 90度の時
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
	mText->Draw("FrontSprite");
}

void ResultUI::SetResultType(const ResultType resultType)
{
	mResultType = resultType;

	switch (mResultType)
	{
	case ResultType::EnemyFelledText:
		mText->SetTexture("EnemyFelledStr");
		break;

	case ResultType::YouDiedText:
		mText->SetTexture("YouDiedStr");
		break;

	default:
		break;
	}
}

bool ResultUI::GetisEnd()
{
	return sIsEnd;
}
