#pragma once
#include "NewEngine.h"
#include "Task.h"
#include "Vignette.h"
#include "Bloom.h"
#include "GaussainBlur.h"
#include "Glare.h"

class TestScene : public IScene
{
private:
	Object3D obj1_;
	Object3D obj2_;

	Sprite spr_;

	Task task_;
	Vignette vignette_;
	Bloom bloom_;
	GaussainBlur gaussainBlur_;
	Glare glare_;

	RenderTexture* backGround_;

	uint32_t postEffectType_;

public:
	TestScene();
	~TestScene();
	void Init() override;
	void Update() override;
	void RenderTextureSetting() override;
	void DrawBackSprite() override;
	void DrawModel() override;
	void DrawFrontSprite() override;
	void DrawRenderTexture() override;
	void DrawDebugGui() override;
};

