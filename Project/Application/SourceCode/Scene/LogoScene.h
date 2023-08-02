#pragma once
#include "NewEngine.h"

class LogoScene : public IScene
{
private:
	std::unique_ptr<Sprite> mLogoSprite;
	std::unique_ptr<Sprite> mBackSprite;

	Timer mStayTimer;

	Easing mAlphaEase;
	bool mIsReverce;
	bool mIsEnd;

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

