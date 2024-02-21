#pragma once
#include "Component.h"
#include "GameObjectInfo.h"

class SpriteInfo : public Component, GameObjectInfo
{
public:
	Vec2 anchorPoint;
	Vec2 spriteSize;
	FlipType flipType;

private:
	void DrawLayerTagGUI();
	void SetParamToObj();

public:
	SpriteInfo();
	SpriteInfo(GameObject* gameObj);
	void Update() override;

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};
