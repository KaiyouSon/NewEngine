#pragma once
#include "Timer.h"

// エフェクトのインタフェース
class IEffect
{
protected:
	bool mIsActive;
	Timer mActiveTimer;

public:
	virtual ~IEffect() {}
	virtual void Update() = 0;
	virtual void Draw() = 0;

public:
	bool GetisActive();
};