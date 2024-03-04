#pragma once
#include "Material.h"
#include "ILight.h"
#include <array>

// 前方宣言
template<typename T> class Singleton;

// ライトを管理するクラス
class LightManager : public Singleton<LightManager>
{
private:
	std::unique_ptr<Material> mMaterial;

private:
	enum LightSize
	{
		DirectionalLightSize = 1,
		PointLightSize = 5,
		SpotLightSize = 1,
	};

private:
	struct CLightGroup
	{
		std::array<ConstantBufferData::CDirectionalLight, DirectionalLightSize> directionalLightsData;
		std::array<ConstantBufferData::CPointLight, PointLightSize> pointLightsData;
		std::array<ConstantBufferData::CSpotLight, SpotLightSize> spotLightsData;
	};

private:
	struct LightGroup
	{
		std::vector<GameObject*> directionalLights;
		std::vector<GameObject*> pointLights;
		std::vector<GameObject*> spotLights;
	};
	LightGroup mLightGroup;

private:
	void MaterialInit();
	void TransferDirectionalLightData(CLightGroup& lightGroupData);
	void TransferPointLightData(CLightGroup& lightGroupData);
	void TransferSpotLightData(CLightGroup& lightGroupData);

public:
	void Init();
	void Update();
	void DrawCommands(const uint32_t index);

public:
	void RegisterLight(GameObject* lightObj);
	void UnRegisterLight(const GameObjectType type);

private:
	friend Singleton<LightManager>;
	LightManager();
};

