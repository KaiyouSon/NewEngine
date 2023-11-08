#include "SpotLight.h"
#include "LightManager.h"

SpotLight::SpotLight() :
	pos(Vec3::up * 5), vec(Vec3::down),
	color(Color::white), colorRate(1), radius(10),
	decay(1), cosAngle(30, 60), isActive(false)
{
	mType = LightType::SpotLight;
	LightManager::GetInstance()->Register(this);
}