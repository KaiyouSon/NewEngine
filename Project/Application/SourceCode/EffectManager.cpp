#include "EffectManager.h"

EffectManager::EffectManager() :
	bloodSpray_(std::make_unique<BloodSpray>())
{
}

void EffectManager::Init()
{
}

void EffectManager::Update()
{
	bloodSpray_->Update();
}

void EffectManager::DrawModel()
{
	bloodSpray_->DrawModel();
}

void EffectManager::GenerateBloodSprayEffect(const Vec3 pos)
{
	bloodSpray_->Generate(pos);
}
