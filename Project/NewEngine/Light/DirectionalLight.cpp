#include "DirectionalLight.h"
#include "LightManager.h"

DirectionalLight::DirectionalLight() :
	pos(Vec3::up), color(Color::white), isActive(true)
{
	mType = LightType::DirectionalLight;
	LightManager::GetInstance()->Register(this);
}