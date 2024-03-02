#pragma once
#include "Component.h"
#include "GameObjectInfo.h"

class CameraInfo : public Component, GameObjectInfo
{
public:
	CameraInfo();
	CameraInfo(GameObject* gameObj);
	void Update() override;

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};