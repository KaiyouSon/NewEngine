#pragma once
#include "Component.h"
#include "GameObjectInfo.h"

class SpotLight;

class SpotLightInfo : public Component, GameObjectInfo
{
private:
	SpotLight* mCastObj;

public:
	SpotLightInfo();
	SpotLightInfo(GameObject* gameObj);
	void Update() override;

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};