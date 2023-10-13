#pragma once
#include "NewEngine.h"
#include "ITransition.h"

// シーン遷移のトランジション
class SceneTransition : public ITransition
{
private:
	std::unique_ptr<Sprite> mTransition;
	std::unique_ptr<Sprite> mLoadSprite;
	Easing mEase;
	Timer mAlphaTimer;

public:
	SceneTransition();
	void Generate() override;
	void Update() override;
	void DrawFrontSprite() override;
};