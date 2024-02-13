#include "TextureAnimation.h"

void TextureAnimation::Init(const Vec2& leftTopPos, const Vec2& rightDownPos)
{
	mInitLeftTopPos = leftTopPos;
	mInitRightDownPos = rightDownPos;
	mLeftTopPos = leftTopPos;
	mRightDownPos = rightDownPos;
	mWidth = mInitRightDownPos.x - mInitLeftTopPos.x;
	isPlay = true;
}

void TextureAnimation::Play(const int& maxTimer, Sprite* sprite)
{
	if (isPlay == false)
	{
		return;
	}

	//if (mAnimetionTimer == true)
	//{
	//	mLeftTopPos.x += mWidth;
	//	mRightDownPos.x += mWidth;
	//	if (mRightDownPos.x > sprite->mTexture->GetBufferResource()->buffer->GetDesc().Width)
	//	{
	//		mLeftTopPos = mInitLeftTopPos;
	//		mRightDownPos = mInitRightDownPos;
	//	}
	//	mAnimetionTimer.Reset();
	//}

	sprite->SetSize(mInitRightDownPos);
	sprite->SetTextureRect(mLeftTopPos, mRightDownPos);

	mAnimetionTimer.SetLimitTimer(maxTimer);
	mAnimetionTimer.Update(true);
}

void TextureAnimation::Reset()
{
	mAnimetionTimer.Reset();
}
