#pragma once
#include "Component.h"

class Object3DInfo : public Component
{
private:
	std::string mChangingName;

public:
	Object3DInfo();
	Object3DInfo(GameObject* gameObj);
	void Update() override;

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};
