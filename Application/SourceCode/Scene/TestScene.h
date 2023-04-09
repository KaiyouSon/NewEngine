#pragma once
#include "NewEngine.h"

class TestScene : public IScene
{
private:

public:
	TestScene();
	~TestScene();
	void Init() override;
	void Update() override;
	void DrawRenderTexture() override;
	void DrawBackSprite() override;
	void DrawModel() override;
	void DrawFrontSprite() override;
	void DrawDebugGui() override;
};

