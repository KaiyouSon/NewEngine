#include "SpotLight.h"
#include "LightManager.h"

SpotLight::SpotLight() :
	vec(Vec3::down), colorRate(1), radius(10),
	decay(1), cosAngle(30, 60)
{
}
SpotLight::SpotLight(const std::string& name) :
	vec(Vec3::down), colorRate(1), radius(10),
	decay(1), cosAngle(30, 60)
{
	this->name = name;

	InitComponents();

	LightManager::GetInstance()->RegisterLight(this);
}

SpotLight::~SpotLight()
{
	LightManager::GetInstance()->UnRegisterLight(mType);
}

void SpotLight::Update()
{
}

void SpotLight::InitComponents()
{
	mType = GameObjectType::SpotLight;

	mComponentManager->AddComponent<SpotLightInfo>();
	mComponentManager->AddComponent<Transform>();
}