#pragma once
#include "NewEngine.h"

class LogoScene : public IScene
{
private:
	std::unique_ptr<Sprite> logoSprite;
	std::unique_ptr<Sprite> backSprite;

	Timer stayTimer;

	Easing alphaEase;
	bool isRevercr;
	bool isEnd;

public:
	LogoScene();
	~LogoScene();
	void Init() override;
	void Update() override;
	void RenderTextureSetting() override;
	void DrawBackSprite() override;
	void DrawModel() override;
	void DrawFrontSprite() override;
	void DrawRenderTexture() override;
	void DrawDebugGui() override;

};

