#pragma once
#include "Component.h"
#include "GameObjectInfo.h"

class ParticleMeshInfo : public Component, GameObjectInfo
{
public:
	ParticleMeshInfo();
	ParticleMeshInfo(GameObject* gameObj);
	void Update() override;
	void CopyComponent(Component* component) override { component; };

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};
