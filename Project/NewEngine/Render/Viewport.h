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

public: // 繧ｻ繝・ち繝ｼ
	void SetViewport(
		const Vec2 leftTopPos, const Vec2 size,
		const float MinDepth = 0.0f, const float MaxDepth = 1.0f);

public: // 繧ｲ繝・ち繝ｼ
	Vec2 GetLeftTopPos();
	Vec2 GetSize();
	float GetMinDepth();
	float GetMaxDepth();
};
