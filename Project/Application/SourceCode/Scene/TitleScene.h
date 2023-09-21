#pragma once
#include "NewEngine.h"
#include "TitleUI.h"
#include "Emitter.h"

class TitleScene : public IScene
{
private:
	std::unique_ptr<TitleUI> mTitleUI;
	float mBgmVolume;

public:
	void Load() override;
	void UnLoad() override;
	void Init() override;
	void Update() override;
	void RenderTextureSetting() override;
	void DrawBackSprite() override;
	void DrawModel() override;
	void DrawFrontSprite() override;
	void DrawRenderTexture() override;
	void DrawDebugGui() override;
};

