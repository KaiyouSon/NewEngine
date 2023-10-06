#pragma once
#include "NewEngine.h"
#include "Player.h"
#include "Boss.h"
#include "UIManager.h"
#include "CameraManager.h"
#include "MenuManager.h"
#include "PostEffectManager.h"
#include "Field.h"
#include "MovieEvent.h"

class GameScene : public IScene
{
private:
	std::unique_ptr<Player> mPlayer;
	std::unique_ptr<Boss> mBoss;
	std::unique_ptr<UIManager> mUiManager;
	std::unique_ptr<MenuManager> mMenuManager;
	std::unique_ptr<PostEffectManager> mPostEffectManager;
	std::unique_ptr<Field> mField;

	std::unique_ptr<MovieEvent> mMovieEvent;

	RenderTexture* mCurrentScene;

	bool mIsChangeScene;

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
