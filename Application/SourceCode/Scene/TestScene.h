#pragma once
#include "NewEngine.h"
#include "Vignette.h"
#include "Bloom.h"
#include "Task.h"

class TestScene : public IScene
{
private:
	Object3D obj1;
	Object3D obj2;

	Sprite spr;

	Task task;
	Vignette vignette;
	Bloom bloom;

	uint32_t postEffectType;

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

