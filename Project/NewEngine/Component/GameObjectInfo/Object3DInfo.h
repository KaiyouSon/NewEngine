#pragma once
#include "Component.h"
#include "GameObjectInfo.h"

class Object3D;

class Object3DInfo : public Component, GameObjectInfo
{
private:
	Object3D* mCastObj;

public:
	Object3DInfo();
	Object3DInfo(GameObject* gameObj);
	void Update() override;

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};
