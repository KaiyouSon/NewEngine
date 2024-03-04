#pragma once
#include "Component.h"
#include "GameObjectInfo.h"

class PointLight;

class PointLightInfo : public Component, GameObjectInfo
{
private:
	PointLight* mCastObj;

public:
	PointLightInfo();
	PointLightInfo(GameObject* gameObj);
	void Update() override;

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};