#include "TextureAnimeiton.h"

void TextureAnimeiton::Init(const Vec2& leftTopPos, const Vec2& rightDownPos)
{
	initLeftTopPos_ = leftTopPos;
	initRightDownPos_ = rightDownPos;
	leftTopPos_ = leftTopPos;
	rightDownPos_ = rightDownPos;
	width_ = initRightDownPos_.x - initLeftTopPos_.x;
	isPlay = true;
}

void TextureAnimeiton::Play(const int& maxTimer, Sprite* sprite)
{
	if (isPlay == false)
	{
		return;
	}

	if (animetionTimer_.GetisTimeOut())
	{
		leftTopPos_.x += width_;
		rightDownPos_.x += width_;
		if (rightDownPos_.x > sprite->texture->buffer->GetDesc().Width)
		{
			leftTopPos_ = initLeftTopPos_;
			rightDownPos_ = initRightDownPos_;
		}
		animetionTimer_.Reset();
	}

	sprite->size = initRightDownPos_;
	sprite->SetTextureRect(leftTopPos_, rightDownPos_);

	animetionTimer_.SetLimitTimer(maxTimer);
	animetionTimer_.Update(true);
}

void TextureAnimeiton::Reset()
{
	animetionTimer_.Reset();
}
