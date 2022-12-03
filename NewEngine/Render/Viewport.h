#pragma once
#include "Vec2.h"
#include <memory>

class Viewport
{
private:
	Vec2 leftTopPos;
	Vec2 size;
	float MinDepth;
	float MaxDepth;

public:
	void Update();

public: // �Z�b�^�[
	inline void SetViewport(const Vec2& leftTopPos, const Vec2& size, float MinDepth = 0.0f, float MaxDepth = 1.0f)
	{
		this->leftTopPos = leftTopPos;
		this->size = size;
		this->MinDepth = MinDepth;
		this->MaxDepth = MaxDepth;
	}

public: // �Q�b�^�[
	inline Vec2 GetLeftTopPos() { return leftTopPos; }
	inline Vec2 GetSize() { return size; }
	inline float GetMinDepth() { return MinDepth; }
	inline float GetMaxDepth() { return MaxDepth; }
};

