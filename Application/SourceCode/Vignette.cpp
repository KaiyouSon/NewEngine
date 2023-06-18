#include "Vignette.h"

Vignette::Vignette() :
	postEffect_(std::make_unique<PostEffect>())
{
	tex_ = TextureManager::GetRenderTexture("Vignette");

	postEffect_->pos = GetWindowHalfSize();
	postEffect_->AddRenderTexture(tex_);
}

void Vignette::Update()
{
	postEffect_->Update();
}

void Vignette::DrawPostEffect()
{
	postEffect_->Draw();
}

void Vignette::PrevSceneDraw()
{
	//tex_->PrevDrawScene();
}

void Vignette::PostSceneDraw()
{
	//tex_->PostDrawScene();
}
