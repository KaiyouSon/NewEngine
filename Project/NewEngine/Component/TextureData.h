#pragma once
#include "StandardLib.h"

class TextureData : public Component
{
private:
	std::vector<ITexture*> mTextures;
	ITexture* mCrrentTex;
	uint32_t mCurrentTexIndex;

public:
	TextureData();
	TextureData(GameObject* gameObj);
	void Update() override;

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;

public:
	void SetTexture(const std::string& tag, const uint32_t index = 0);
	void SetTexture(ITexture* tex, const uint32_t index = 0);
	void SetCurrentTexture(const std::string& tag);
	ITexture* GetTexture(const uint32_t index = 0);
	ITexture* GetCurrentTexture();
};