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
#include "VolumetricFog.h"
#include "Skydome.h"
#include "Trajectory.h"
#include "ParticleObject.h"

// ゲームシーンのクラス
class GameScene : public IScene
{
private:
	std::unique_ptr<Player> mPlayer;
	std::unique_ptr<Boss> mBoss;
	std::unique_ptr<UIManager> mUiManager;
	std::unique_ptr<MenuManager> mMenuManager;
	std::unique_ptr<PostEffectManager> mPostEffectManager;
	std::unique_ptr<Field> mField;
	std::unique_ptr<Skydome> mSkydome;

	std::unique_ptr<MovieEvent> mMovieEvent;
	std::unique_ptr<VolumetricFog> mVolumetricFog;
	std::unique_ptr<DirectionalLight> mDirectionalLight;

	ParticleObject mParticleObject;
	Object3D obj;

	Trajectory mTrajectory;

	RenderTexture* mCurrentScene;

	bool mIsChangeScene;

	bool isInit;
	float mBgmVolume;

private:
	// シーン切り替えの処理
	void SceneChangeUpdate();

private:
	// 天球の描画
	void DrawSkydome();

	// エフェクトのブルームのポストエフェクトに深度のみ書き込む処理
	void DrawDepthToEffectBloom();

	// 現在のシーンのオブジェクトの描画
	void DrawCurrentSceneObject();

public:
	void Load() override;
	void UnLoad() override;
	void CreateInstance() override;
	void Init() override;
	void Update() override;
	void DrawPass() override;
	void Draw() override;
	void DrawDebugGui() override;
};
