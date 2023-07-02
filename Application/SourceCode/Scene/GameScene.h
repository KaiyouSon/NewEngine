#pragma once
#include "NewEngine.h"
#include "Player.h"
#include "UIManager.h"
#include "CameraManager.h"

class GameScene : public IScene
{
private:
	std::unique_ptr<Player> player;
	std::unique_ptr<UIManager> uiManager;
	std::unique_ptr<CameraManager> cameraManager;

	std::unique_ptr<Object3D> ground;

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

