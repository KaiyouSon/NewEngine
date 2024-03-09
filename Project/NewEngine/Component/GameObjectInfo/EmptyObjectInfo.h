#pragma once
#include "Component.h"
#include "GameObjectInfo.h"

class EmptyObjectInfo : public Component, GameObjectInfo
{
public:
	EmptyObjectInfo();
	EmptyObjectInfo(GameObject* gameObj);
	void Update() override;
	void CopyComponent([[maybe_unused]] Component* component) override {}

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};
