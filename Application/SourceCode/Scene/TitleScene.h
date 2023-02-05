#pragma once
#include "NewEngine.h"
#include "CircleGaugeSprite.h"

class TitleScene : public IScene
{
private:
	Object3D obj;

	CircleGaugeSprite spr;

public:
	void Init() override;
	void Update() override;
	void DrawRenderTexture() override;
	void DrawBackSprite() override;
	void DrawModel() override;
	void DrawFrontSprite() override;
	void DrawDebugGui() override;
};

