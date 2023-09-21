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
	std::unique_ptr<Player> mPlayer;
	std::unique_ptr<Boss> mBoss;
	std::unique_ptr<UIManager> mUiManager;
	std::unique_ptr<CameraManager> mCameraManager;
	std::unique_ptr<MenuManager> mMenuManager;
	std::unique_ptr<Field> mField;

	bool isInit;
	float mBgmVolume;

public:
	GameScene();
	~GameScene();
	void Load() override;
	void UnLoad() override;
	void CreateInstance() override;
	void Init() override;
	void Update() override;
	void RenderTextureSetting() override;
	void Draw() override;
	void DrawDebugGui() override;
};

