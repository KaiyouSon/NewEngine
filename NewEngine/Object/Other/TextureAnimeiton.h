#pragma once
#include "Sprite.h"
#include "Timer.h"

class Sprite;

class TextureAnimeiton
{
private:
	Timer animetionTimer_;
	Vec2 initLeftTopPos_;
	Vec2 initRightDownPos_;
	Vec2 leftTopPos_;
	Vec2 rightDownPos_;
	float width_;

public:
	bool isPlay;

public:
	void Init(const Vec2& leftTopPos, const Vec2& rightDownPos);
	void Play(const int& maxTimer, Sprite* sprite);
	void Reset();
};

