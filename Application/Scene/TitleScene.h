#pragma once
#include "NewEngine.h"

class TitleScene : public IScene
{
private:
	Object3D obj;

public:
	void Init() override;
	void Update() override;
	void DrawBackSprite() override;
	void DrawModel() override;
	void DrawFrontSprite() override;
	void DrawDebugGui() override;
};

