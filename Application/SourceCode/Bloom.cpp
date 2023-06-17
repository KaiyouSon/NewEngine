#include "Bloom.h"

Bloom::Bloom() :
	postEffect_(std::make_unique<PostEffect>())
{
	tex_ = TextureManager::GetRenderTexture("Bloom");

	postEffect_->pos = GetWindowHalfSize();
	postEffect_->SetRenderTexture(tex_);
}

void Bloom::Update()
{
	postEffect_->Update();
}

void Bloom::DrawPostEffect()
{
	postEffect_->Draw();
}

void Bloom::PrevSceneDraw()
{
	tex_->PrevDrawScene();
}

void Bloom::PostSceneDraw()
{
	tex_->PostDrawScene();
}
