#include "TextureAnimeiton.h"

void TextureAnimeiton::Init(const Vec2& leftTopPos, const Vec2& rightDownPos)
{
	initLeftTopPos = leftTopPos;
	initRightDownPos = rightDownPos;
	this->leftTopPos = leftTopPos;
	this->rightDownPos = rightDownPos;
	width = initRightDownPos.x - initLeftTopPos.x;
	isPlay = true;
}

void TextureAnimeiton::Play(const int& maxTimer, Sprite* sprite)
{
	if (isPlay == false)
	{
		return;
	}

	if (animetionTimer.GetisTimeOut())
	{
		leftTopPos.x += width;
		rightDownPos.x += width;
		if (rightDownPos.x > sprite->texture.buffer->GetDesc().Width)
		{
			leftTopPos = initLeftTopPos;
			rightDownPos = initRightDownPos;
		}
		animetionTimer.Reset();
	}

	sprite->size = initRightDownPos;
	sprite->SetTextureRect(leftTopPos, rightDownPos);

	animetionTimer.SetMaxTimer(maxTimer);
	animetionTimer.AddTimer();
}

void TextureAnimeiton::Reset()
{
	animetionTimer.Reset();
}
