#include "PointLight.h"
#include "LightManager.h"

PointLight::PointLight() :
	pos(Vec3::up), atten(Vec3(0.3f, 0.1f, 0.1f)),
	length(10.f), color(Color::white), isActive(true)
{
	mType = LightType::PointLight;
	LightManager::GetInstance()->Register(this);
}