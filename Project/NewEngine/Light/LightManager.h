#pragma once
#include "Material.h"
#include "ILight.h"
#include <array>

// スポットライト
struct SpotLight
{
	Vec3 vec = Vec3::down;
	Vec3 pos = Vec3::up * 5;
	Color color = Color::white;
	Vec3 atten = Vec3::zero;
	Vec2 factorAngle = { 20,30 };
	bool isActive = false;

	SpotLight() {}
	~SpotLight() {}
};

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
		PointLightSize = 20,
	};

private:
	struct CLightGroup
	{
		std::array<ConstantBufferData::CDirectionalLight, DirectionalLightSize> directionalLightsData;
		std::array<ConstantBufferData::CPointLight, PointLightSize> pointLightsData;
	};


private:
	struct LightGroup
	{
		std::vector<ILight*> directionalLights;
		std::vector<ILight*> pointLights;
	};
	LightGroup mLightGroup;

private:
	void MaterialInit();

public:
	void Init();
	void Update();
	void DrawCommand(const uint32_t index);

public:
	void Register(ILight* iLight);

private:
	friend Singleton<LightManager>;
	LightManager();
};

