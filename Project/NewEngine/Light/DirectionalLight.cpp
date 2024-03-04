#include "DirectionalLight.h"
#include "LightManager.h"

DirectionalLight::DirectionalLight()
{
}

DirectionalLight::DirectionalLight(const std::string& name)
{
	this->name = name;

	//コンポネントの初期化
	InitComponents();

	LightManager::GetInstance()->RegisterLight(this);
}

DirectionalLight::~DirectionalLight()
{
	LightManager::GetInstance()->UnRegisterLight(mType);
}

void DirectionalLight::Update()
{
}

void DirectionalLight::InitComponents()
{
	mType = GameObjectType::DirectionalLight;

	mComponentManager->AddComponent<DirectionalLightInfo>();
	mComponentManager->AddComponent<Transform>();
}