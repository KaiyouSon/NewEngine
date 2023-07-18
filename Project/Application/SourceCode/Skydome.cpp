#include "Skydome.h"

Skydome::Skydome() :
	skydome_(std::make_unique<Object3D>())
{
	skydome_->SetModel(ModelManager::GetModel("Skydome"));
	//skydome_->color = Color(0xc4c178);
	skydome_->isLighting = false;

	for (uint32_t i = 0; i < 10; i++)
	{
		clouds_.emplace_back(std::move(std::make_unique<Cloud>()));

		const float range = 150.f;
		const Vec3 randomPos =
		{
			Random::RangeF(-range, range),
			150.f + i,
			Random::RangeF(-range, range),
		};
		clouds_.back()->SetPos(randomPos);
	}
}

void Skydome::Init()
{
	skydome_->scale = 500.f;
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
