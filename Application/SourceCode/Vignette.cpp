#include "Vignette.h"

Vignette::Vignette() :
	postEffect(std::make_unique<PostEffect>())
{
	vignetteTex = TextureManager::GetRenderTexture("Test");

	postEffect->AddRenderTexture(vignetteTex);
}

void Vignette::Update()
{
	postEffect->Update();
}

void Vignette::DrawPostEffect()
{
	postEffect->Draw();
}

void Vignette::PrevSceneDraw()
{
	vignetteTex->PrevDrawScene();
}

void Vignette::PostSceneDraw()
{
	vignetteTex->PostDrawScene();
}
