#include "PointLight.h"
#include "LightManager.h"

PointLight::PointLight() :
	pos(Vec3::up), colorRate(Vec3::one),
	radius(10.f), color(Color::white), isActive(true)
{
	mType = LightType::PointLight;
	LightManager::GetInstance()->Register(this);
}