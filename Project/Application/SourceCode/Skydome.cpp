#include "Skydome.h"

Skydome::Skydome() :
	skydome_(std::make_unique<Object3D>())
{
	skydome_->SetModel(ModelManager::GetModel("Skydome"));
	skydome_->isLighting = false;
}

void Skydome::Init()
{
	skydome_->scale = 500.f;
}

void Skydome::Update()
{
	skydome_->Update();
}

void Skydome::DrawModel()
{
	skydome_->Draw();
}
