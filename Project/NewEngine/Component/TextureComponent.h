#pragma once
#include "StandardLib.h"

class TextureComponent : public Component
{
private:
	std::vector<ITexture*> mTextures;
	ITexture* mTexture;

public:
	TextureComponent();
	TextureComponent(GameObject* gameObj);
	void Update() override;
	void CopyComponent(Component* component) override { component; };

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;

public:
	void SetTexture(const std::string& tag);
	void SetTexture(ITexture* tex);
	ITexture* GetTexture();
};