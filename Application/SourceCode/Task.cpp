#include "Task.h"

Task::Task() :
	postEffect_(std::make_unique<PostEffect>())
{
	tex_ = TextureManager::GetRenderTexture("Task");

	postEffect_->pos = GetWindowHalfSize();
	postEffect_->AddRenderTexture(tex_);
	postEffect_->AddRenderTexture(tex_);
}

void Task::Update()
{
	postEffect_->Update();
}

void Task::DrawPostEffect()
{
	postEffect_->Draw();
}

void Task::PrevSceneDraw()
{
	tex_->PrevDrawScene();
}

void Task::PostSceneDraw()
{
	tex_->PostDrawScene();
}
