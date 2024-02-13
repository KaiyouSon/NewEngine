#pragma once
#include "Component.h"

class SpriteInfo : public Component
{
private:
	std::string mChangingName;

public:
	Vec2 anchorPoint;
	Vec2 spriteSize;
	FlipType flipType;

public:
	SpriteInfo();
	SpriteInfo(GameObject* gameObj);
	void Update() override;

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};
