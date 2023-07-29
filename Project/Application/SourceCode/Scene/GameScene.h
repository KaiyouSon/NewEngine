#pragma once
#include "NewEngine.h"
#include "Player.h"
#include "Boss.h"
#include "UIManager.h"
#include "CameraManager.h"
#include "MenuManager.h"
#include "Field.h"

class GameScene : public IScene
{
private:
	std::unique_ptr<Player> player_;
	std::unique_ptr<Boss> boss_;
	std::unique_ptr<UIManager> uiManager_;
	std::unique_ptr<CameraManager> cameraManager_;
	std::unique_ptr<MenuManager> menuManager_;
	std::unique_ptr<Field> field_;

	std::unique_ptr<PostEffect> currentScene_;
	RenderTexture* tex_;

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

