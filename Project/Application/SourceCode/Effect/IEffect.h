#pragma once

// エフェクトのインタフェース
class IEffect
{
protected:
	bool mIsActive;

public:
	virtual ~IEffect() {}
	virtual void Update() = 0;
	virtual void Draw() = 0;

public:
	bool GetisActive();
};