#pragma once
#include "StandardLib.h"

class TextureData : public Component
{
private:
	std::vector<ITexture*> mTextures;
	ITexture* mCurrentTex;
	uint32_t mCurrentTexIndex;

public:
	TextureData();
	TextureData(GameObject* gameObj);
	void Update() override;
	void CopyComponent(Component* component) override { component; };

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;

public:
	void SetTexture(const std::string& tag, const uint32_t index = 0);
	void SetTexture(ITexture* tex, const uint32_t index = 0);
	void SetCurrentTexture(const std::string& tag);
	void SetCurrentTexture(ITexture* tex);
	ITexture* GetTexture(const uint32_t index = 0);
	ITexture* GetCurrentTexture();
};