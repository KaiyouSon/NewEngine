#pragma once
#include "StandardLib.h"

class TextureComponent : public Component
{
private:
	ITexture* mTexture;

public:
	TextureComponent();
	TextureComponent(GameObject* gameObj);
	void Update() override;
	void CopyComponent(Component* component) override;

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;

public:
	void SetTexture(const std::string& tag);
	void SetTexture(ITexture* tex);
	ITexture* GetTexture();
};