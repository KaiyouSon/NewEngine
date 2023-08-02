#pragma once
#include "Vec2.h"

class Viewport
{
private:
	Vec2 mLeftTopPos;
	Vec2 mSize;
	float mMinDepth;
	float mMaxDepth;

public:
	Viewport();
	void Update();

public: // セッター
	void SetViewport(
		const Vec2 leftTopPos, const Vec2 size,
		const float MinDepth = 0.0f, const float MaxDepth = 1.0f);

public: // ゲッター
	Vec2 GetLeftTopPos();
	Vec2 GetSize();
	float GetMinDepth();
	float GetMaxDepth();
};

