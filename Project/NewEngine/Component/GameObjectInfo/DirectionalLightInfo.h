#pragma once
#include "Component.h"
#include "GameObjectInfo.h"

class DirectionalLight;

class DirectionalLightInfo : public Component, GameObjectInfo
{
private:
	DirectionalLight* mCastObj;

public:
	DirectionalLightInfo();
	DirectionalLightInfo(GameObject* gameObj);
	void Update() override;
	void CopyComponent(Component* component) override { component; };

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};