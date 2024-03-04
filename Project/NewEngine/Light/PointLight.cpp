#include "PointLight.h"
#include "LightManager.h"

PointLight::PointLight() :
	colorRate(Vec3::one), radius(10.f), decay(1.f)
{
}

PointLight::PointLight(const std::string& name) :
	colorRate(Vec3::one), radius(10.f), decay(1.f)
{
	this->name = name;

	InitComponents();

	LightManager::GetInstance()->RegisterLight(this);
}

PointLight::~PointLight()
{
	LightManager::GetInstance()->UnRegisterLight(mType);
}

void PointLight::Update()
{
}

void PointLight::InitComponents()
{
	mType = GameObjectType::PointLight;

	mComponentManager->AddComponent<PointLightInfo>();
	mComponentManager->AddComponent<Transform>();
}