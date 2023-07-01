#pragma once
#include "NewEngine.h"
#include "Player.h"
#include "UIManager.h"

class GameScene : public IScene
{
private:
	std::unique_ptr<Player> player;
	std::unique_ptr<UIManager> uiManager;

public:
	GameScene();
	~GameScene();
	void Init() override;
	void Update() override;
	void RenderTextureSetting() override;
	void DrawBackSprite() override;
	void DrawModel() override;
	void DrawFrontSprite() override;
	void DrawRenderTexture() override;
	void DrawDebugGui() override;
};

