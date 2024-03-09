#pragma once
#include "Component.h"
#include "GameObjectInfo.h"

class CameraInfo : public Component, GameObjectInfo
{
public:
	CameraInfo();
	CameraInfo(GameObject* gameObj);
	void Update() override;
	void CopyComponent(Component* component) override { component; };

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};