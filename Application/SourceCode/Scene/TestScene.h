#pragma once
#include "NewEngine.h"

class TestScene : public IScene
{
private:
	Object3D obj1;
	Object3D obj2;

	Sprite spr1;
	Sprite spr2;

	RenderTexture* renderTex;

public:
	TestScene();
	~TestScene();
	void Init() override;
	void Update() override;
	void DrawRenderTexture() override;
	void DrawBackSprite() override;
	void DrawModel() override;
	void DrawFrontSprite() override;
	void DrawDebugGui() override;
};

