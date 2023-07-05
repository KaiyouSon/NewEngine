#pragma once
#include "Vec2.h"

class Viewport
{
private:
	Vec2 leftTopPos_;
	Vec2 size_;
	float minDepth_;
	float maxDepth_;

public:
	Viewport();
	void Update();

public: // セッター
	inline void SetViewport(const Vec2& leftTopPos, const Vec2& size, float MinDepth = 0.0f, float MaxDepth = 1.0f)
	{
		leftTopPos_ = leftTopPos;
		size_ = size;

		minDepth_ = MinDepth;
		maxDepth_ = MaxDepth;
	}

public: // ゲッター
	inline Vec2 GetLeftTopPos() { return leftTopPos_; }
	inline Vec2 GetSize() { return size_; }
	inline float GetMinDepth() { return minDepth_; }
	inline float GetMaxDepth() { return maxDepth_; }
};

