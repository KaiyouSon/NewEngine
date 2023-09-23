#pragma once
#include "NewEngine.h"
#include "ITransition.h"

class SceneTransition : public ITransition
{
private:
	std::unique_ptr<Sprite> mTransition;
	Easing mEase;

public:
	SceneTransition();
	void Generate() override;
	void Update() override;
	void DrawFrontSprite() override;

};

