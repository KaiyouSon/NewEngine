#include "Task.h"

Task::Task() :
	postEffect_(std::make_unique<PostEffect>())
{
	tex_ = TextureManager::GetRenderTexture("Task");

	postEffect_->pos = GetWindowHalfSize();
	postEffect_->AddRenderTexture(tex_);
	//postEffect_->AddRenderTexture(tex_);
	postEffect_->rtvIndex = 0;
}

void Task::Update()
{
	if (Key::GetKeyDown(DIK_SPACE))
	{
		if (postEffect_->rtvIndex == 0)
		{
 			postEffect_->rtvIndex = 1;
		}
		else if (postEffect_->rtvIndex == 1)
		{
			postEffect_->rtvIndex = 0;
		}
	}

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
