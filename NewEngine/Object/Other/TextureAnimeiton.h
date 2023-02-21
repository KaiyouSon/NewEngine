#pragma once
#include "Sprite.h"
#include "Timer.h"

class TextureAnimeiton
{
private:
	Timer animetionTimer;
	Vec2 initLeftTopPos;
	Vec2 initRightDownPos;
	Vec2 leftTopPos;
	Vec2 rightDownPos;
	float width;

public:
	bool isPlay;

public:
	void Init(const Vec2& leftTopPos, const Vec2& rightDownPos);
	void Play(const int& maxTimer, Sprite* sprite);
	void Reset();
};

