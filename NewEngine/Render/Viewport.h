#pragma once
#include "Vec2.h"
#include <memory>

class Viewport
{
private:
	Vec2 leftTopPos;
	Vec2 size;
	float minDepth;
	float maxDepth;

public:
	Viewport();
	void Update();

public: // セッター
	inline void SetViewport(const Vec2& leftTopPos, const Vec2& size, float MinDepth = 0.0f, float MaxDepth = 1.0f)
	{
		this->leftTopPos = leftTopPos;
		this->size = size;
		this->minDepth = MinDepth;
		this->maxDepth = MaxDepth;
	}

public: // ゲッター
	inline Vec2 GetLeftTopPos() { return leftTopPos; }
	inline Vec2 GetSize() { return size; }
	inline float GetMinDepth() { return minDepth; }
	inline float GetMaxDepth() { return maxDepth; }
};

