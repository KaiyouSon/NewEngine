#pragma once
#include "Component.h"

class MaterialComponent : public Component
{
private:
	std::unique_ptr<Material> mMaterial;

public:
	MaterialComponent();
	MaterialComponent(GameObject* gameObj);
	void Update() override;
	void CopyComponent(Component* component) override;

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;

public:
	// セッター
	void SetMaterial(const std::string& name);

public:
	// ゲッター
	Material* GetMaterial();

};

