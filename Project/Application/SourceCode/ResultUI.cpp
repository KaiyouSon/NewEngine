#include "ResultUI.h"

bool ResultUI::isEnd_ = false;

ResultUI::ResultUI() :
	back_(std::make_unique<Sprite>()),
	text_(std::make_unique<Sprite>()),
	textAfterImage_(std::make_unique<Sprite>())
{
	back_->SetTexture(TextureManager::GetTexture("ResultBack"));
	text_->SetTexture(TextureManager::GetTexture("YouDiedStr"));
	textAfterImage_->SetTexture(TextureManager::GetTexture("EnemyFelledStr"));
}
void ResultUI::Init()
{
	back_->scale.x = 15.f;
	back_->color.a = 0.f;
	text_->color.a = 0.f;

	parent_.pos = GetWindowHalfSize();
	parent_.Update();

	timer_.SetLimitTimer(180);

	isAfterImage_ = false;
	textAfterImage_->color.a = 0.f;

	isEnd_ = false;
}
void ResultUI::Update()
{
	if (isActive_ == true)
	{
		switch (resultType_)
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

	back_->Update(&parent_);
	text_->Update(&parent_);
	textAfterImage_->Update(&parent_);
}
void ResultUI::DrawFrontSprite()
{
	back_->Draw();

	switch (resultType_)
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
	if (isEnd_ == true)
	{
		return;
	}
	waitTimer_.SetLimitTimer(120);

	if (isWait_ == true)
	{
		waitTimer_.Update(false);
		if (waitTimer_.GetisTimeOut() == true)
		{
			waitTimer_.Reset();
			isWait_ = false;
		}
	}
	else
	{
		back_->color.a = sinf(Radian((float)timer_.GetTimer())) * 245.f;
		text_->color.a = sinf(Radian((float)timer_.GetTimer())) * 245.f;

		if (isAfterImage_ == false)
		{
			text_->scale += 0.001f;
		}

		timer_.Update(false);
		// 90“x‚ÌŽž
		if (timer_.GetTimer() == 90)
		{
			if (isAfterImage_ == false)
			{
				isAfterImage_ = true;
				textAfterImage_->scale = text_->scale;
				//textAfterImage_->color.a = 105.f;
			}

			if (isWait_ == false)
			{
				isWait_ = true;
			}
		}
	}

	if (isAfterImage_ == true)
	{
		textAfterImage_->scale += 0.0005f;
		textAfterImage_->color.a = sinf(Radian((float)timer_.GetTimer())) * 55.f;
	}

	if (timer_.GetisTimeOut() == true)
	{
		back_->color.a = 0;
		text_->color.a = 0;
		textAfterImage_->color.a = 0;
		text_->scale = 1;

		timer_.Reset();
		isActive_ = false;
		isAfterImage_ = false;
		isEnd_ = true;
	}
}
void ResultUI::EnemyFelledDraw()
{
	textAfterImage_->Draw();
	text_->Draw();
}

void ResultUI::YouDiedUpdate()
{
	if (isEnd_ == true)
	{
		return;
	}

	waitTimer_.SetLimitTimer(60);

	if (isWait_ == true)
	{
		waitTimer_.Update(false);
		if (waitTimer_.GetisTimeOut() == true)
		{
			waitTimer_.Reset();
			isWait_ = false;
		}
	}
	else
	{
		back_->color.a = sinf(Radian((float)timer_.GetTimer())) * 245.f;
		text_->color.a = sinf(Radian((float)timer_.GetTimer())) * 245.f;
		text_->scale += 0.001f;

		timer_.Update(false);
		// 90“x‚ÌŽž
		if (timer_.GetTimer() == 90)
		{
			if (isWait_ == false)
			{
				isWait_ = true;
			}
		}
	}

	if (timer_.GetisTimeOut() == true)
	{
		back_->color.a = 0;
		text_->color.a = 0;
		text_->scale = 1;

		timer_.Reset();
		isActive_ = false;
		isEnd_ = true;
	}
}
void ResultUI::YouDiedDraw()
{
	text_->Draw();
}

void ResultUI::SetisActive(const bool isActive)
{
	isActive_ = isActive;
}
void ResultUI::SetResultType(const ResultType resultType)
{
	resultType_ = resultType;

	switch (resultType_)
	{
	case ResultType::EnemyFelledStr:
		text_->SetTexture(TextureManager::GetTexture("EnemyFelledStr"));
		break;

	case ResultType::YouDiedStr:
		text_->SetTexture(TextureManager::GetTexture("YouDiedStr"));
		break;

	default:
		break;
	}
}

bool ResultUI::GetisEnd()
{
	return isEnd_;
}
