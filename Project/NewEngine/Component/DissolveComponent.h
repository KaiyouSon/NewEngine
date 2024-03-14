#pragma once
#include "Component.h"

class DissolveComponent : public Component
{
private:
	struct CBData
	{
		Color dissolveColor;
		float dissolve;
		float colorPower;
	};

private:
	ITexture* mTexture;
	Color dissolveColor;
	float dissolve;
	float colorPower;
	Color colorRate;
	std::unique_ptr<ConstantBuffer<CBData>> mConstantBuffer;

public:
	DissolveComponent();
	DissolveComponent(GameObject* gameObj);
	void Update() override;
	void CopyComponent(Component* component) override;

	void TransferData();
	void DrawCommands(const uint32_t& index);

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;

public:
	// セッター
	void SetTexture(const std::string& tag);

public:
	// ゲッター
	ITexture* GetTexture();
};

