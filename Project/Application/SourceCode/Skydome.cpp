#include "Skydome.h"

Skydome::Skydome() :
	skydome_(std::make_unique<Object3D>())
{
	skydome_->SetModel(ModelManager::GetModel("Skydome"));
	skydome_->color = Color(0xc4c178);
	skydome_->isLighting = false;

	for (uint32_t i = 0; i < 50; i++)
	{
		clouds_.emplace_back(std::move(std::make_unique<Cloud>()));

		const float range = 500.f;
		const Vec3 randomPos =
		{
			Random::RangeF(-range, range),
			150.f,
			Random::RangeF(-range, range),
		};
		clouds_.back()->SetPos(randomPos);
	}
}

void Skydome::Init()
{
	skydome_->scale = 800.f;
}

void Skydome::Update()
{
	skydome_->Update();
	for (uint32_t i = 0; i < clouds_.size(); i++)
	{
		clouds_[i]->Update();
	}
}

void Skydome::DrawModel()
{
	skydome_->Draw();
	for (uint32_t i = 0; i < clouds_.size(); i++)
	{
		clouds_[i]->DrawModel();
	}
}

void Skydome::DrawDebugGui()
{
	GuiManager::BeginWindow("Skydome");
	GuiManager::DrawColorEdit("Skydome Color", skydome_->color);
	GuiManager::EndWindow();
}
