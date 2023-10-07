#pragma once
#include "NewEngine.h"

class RespawnPointUI
{
private:
	Transform mParent;
	std::unique_ptr<Sprite> mBack;
	std::array<std::unique_ptr<Sprite>, 1> mText;
	bool mIsActive;
	float mAlpha;

public:
	RespawnPointUI();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetisActive(const bool isActive);

public:
	bool GetisActive();
};

