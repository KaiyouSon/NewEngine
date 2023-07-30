#pragma once
#include "NewEngine.h"
#include "TitleUI.h"
#include "Emitter.h"

class TitleScene : public IScene
{
private:
	std::unique_ptr<TitleUI> titleUI_;
	float bgmVolume;

public:
	void Init() override;
	void Update() override;
	void RenderTextureSetting() override;
	void DrawBackSprite() override;
	void DrawModel() override;
	void DrawFrontSprite() override;
	void DrawRenderTexture() override;
	void DrawDebugGui() override;
};

