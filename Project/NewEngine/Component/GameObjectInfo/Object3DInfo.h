#pragma once
#include "Component.h"
#include "GameObjectInfo.h"

class Object3DInfo : public Component, GameObjectInfo
{
public:
	Object3DInfo();
	Object3DInfo(GameObject* gameObj);
	void Update() override;
	void CopyComponent(Component* component) override { component; };

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};
