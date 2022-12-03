#pragma once
#include "imgui.h"
#include "MathUtil.h"
class ILayer
{
protected:
	Vec2 pos;
	Vec2 size;

public:
	virtual ~ILayer() {}
	virtual void Initialize() = 0;
	virtual void Update() = 0;

public:	// ÉQÉbÉ^Å[
	inline Vec2 GetPos() { return pos; }
	inline Vec2 GetSize() { return size; }
};
