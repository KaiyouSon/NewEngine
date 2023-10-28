#pragma once
#include "Sprite.h"
#include "Timer.h"

// 前方宣言
class Sprite;

// テクスチャのアニメーションクラス
class TextureAnimation
{
private:
	Timer mAnimetionTimer;
	Vec2 mInitLeftTopPos;
	Vec2 mInitRightDownPos;
	Vec2 mLeftTopPos;
	Vec2 mRightDownPos;
	float mWidth;

public:
	bool isPlay;

public:
	void Init(const Vec2& leftTopPos, const Vec2& rightDownPos);
	void Play(const int& maxTimer, Sprite* sprite);
	void Reset();
};

