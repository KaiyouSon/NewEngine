#pragma once
#include "NewEngine.h"

class GameScene : public IScene
{
private:

public:
	GameScene();
	~GameScene();
	void Init() override;
	void Update() override;
	void DrawRenderTexture() override;
	void DrawBackSprite() override;
	void DrawModel() override;
	void DrawFrontSprite() override;
	void DrawDebugGui() override;
};

