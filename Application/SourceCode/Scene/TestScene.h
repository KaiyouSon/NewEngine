#pragma once
#include "NewEngine.h"
#include "Task.h"
#include "Vignette.h"
#include "Bloom.h"
#include "GaussianBlur.h"
#include "Glare.h"
#include "DOF.h"

class TestScene : public IScene
{
private:
	std::vector<Object3D> objs_;
	Object3D obj1_;
	Object3D obj2_;
	Object3D skyDome_;

	Sprite spr_;

	Task task_;
	Vignette vignette_;
	Bloom bloom_;
	GaussianBlur gaussianBlur_;
	Glare glare_;
	DOF dof_;

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

