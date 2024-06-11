#pragma once
#include "Component.h"
#include "GameObjectInfo.h"

class PostProcessInfo : public Component, GameObjectInfo
{
public:
	Vec2 anchorPoint;
	Vec2 spriteSize;
	FlipType flipType;
	RectAngle rect;

private:
	void SetParamToObj();

private:
	nlohmann::json SavePostProcessInfoToJson();
	void LoadPostProcessInfoToJson(const nlohmann::json& componentField);

public:
	PostProcessInfo();
	PostProcessInfo(GameObject* gameObj);
	void Update() override;
	void CopyComponent(Component* component) override;

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};
