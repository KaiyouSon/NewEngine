#include "EffectManager.h"

EffectManager::EffectManager() :
	bloodSpray_(std::make_unique<BloodSpray>()),
	playerRecoveryEffect_(std::make_unique<PlayerRecoveryEffect>())
{
}

void EffectManager::Init()
{
}

void EffectManager::Update()
{
	bloodSpray_->Update();
	playerRecoveryEffect_->Update();
}

void EffectManager::DrawModel()
{
	bloodSpray_->DrawModel();
	playerRecoveryEffect_->DrawModel();
}

void EffectManager::GenerateBloodSprayEffect(const Vec3 pos)
{
	bloodSpray_->Generate(pos);
}

void EffectManager::GeneratePlayerRecoveryEffect(const Vec3 pos)
{
	playerRecoveryEffect_->Generate(pos);
}
