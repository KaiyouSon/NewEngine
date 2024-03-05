#pragma once
#include "Component.h"
#include "GameObjectInfo.h"

class ParticleMeshInfo : public Component, GameObjectInfo
{
public:
	ParticleMeshInfo();
	ParticleMeshInfo(GameObject* gameObj);
	void Update() override;

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};
