#pragma once
#include "json.hpp"

class GameObject;

struct ComponentInfo
{
	uint32_t index;
	ComponentType type;

	void LoadToJson(const nlohmann::json& componentInfoField);
	nlohmann::json SaveToJson();
};

class Component
{
protected:
	ComponentInfo mComponentInfo;
	GameObject* mGameObj;

public:
	Component();
	Component(GameObject* gameObj);
	virtual ~Component() {}
	virtual void Update() = 0;
	virtual void CopyComponent(Component* component) = 0;

public:
	virtual nlohmann::json SaveToJson() = 0;
	virtual void LoadToJson(const nlohmann::json& componentField) = 0;
	virtual void ShowDataToInspector() = 0;

public:
	// セッター
	void SetComponentInfo(const uint32_t index);

public:
	// ゲッター
	ComponentType GetComponentType();
};