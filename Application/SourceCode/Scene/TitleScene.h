#pragma once
#include "NewEngine.h"

class TitleScene : public IScene
{
private:
	TextureAnimeiton anime;

	Object3D obj;
	Object3D fbx;
	Object3D skyDome;
	Sprite sprite;
	SphereCollider sphereCollider;

public:
	void Init() override;
	void Update() override;
	void DrawRenderTexture() override;
	void DrawBackSprite() override;
	void DrawModel() override;
	void DrawFrontSprite() override;
	void DrawDebugGui() override;
};

