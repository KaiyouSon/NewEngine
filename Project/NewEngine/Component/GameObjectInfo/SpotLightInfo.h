#pragma once
#include "Component.h"
#include "GameObjectInfo.h"

class SpotLight;

class SpotLightInfo : public Component, GameObjectInfo
{
private:
	SpotLight* mCastObj;

private:
	nlohmann::json SaveSpotLightInfoToJson();
	void LoadSpotLightInfoToJson(const nlohmann::json& componentField);

public:
	SpotLightInfo();
	SpotLightInfo(GameObject* gameObj);
	void Update() override;
	void CopyComponent(Component* component) override { component; };

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};