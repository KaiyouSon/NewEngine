#pragma once
#include "NewEngine.h"
#include "TitleUI.h"
#include "PostEffectManager.h"

// タイトルシーンのクラス
class TitleScene : public IScene
{
private:
	std::unique_ptr<TitleUI> mTitleUI;
	std::unique_ptr<PostEffectManager> mPostEffectManager;
	float mBgmVolume;
	bool mIsPush;

private:
	Timer mTimer;

private:
	// シーン切り替えの処理
	void SceneChangeUpdate();

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
