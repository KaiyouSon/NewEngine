#pragma once
#include "CircleShadow.h"
#include "Fog.h"
#include <array>

struct DirectionalLight
{
	Vec3 dirVec = Vec3::up;
	Color color = Color::white;
	bool isActive = false;

	DirectionalLight() {}
	~DirectionalLight() {}
};

struct PointLight
{
	Vec3 pos = Vec3::up;
	Color color = Color::white;
	Vec3 atten = { 0.3f,0.1f,0.1f };
	bool isActive = false;

	PointLight() {}
	~PointLight() {}
};

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

template<typename T> class Singleton;
class LightManager : public Singleton<LightManager>
{
private:
	static const uint32_t sDirectionalLightNum_ = 1;
	static const uint32_t sPointLightNum_ = 3;
	static const uint32_t sSpotLightNum_ = 3;

	struct ConstantBufferDataLightManager
	{
		std::array<ConstantBufferDataDirectionalLight, sDirectionalLightNum_> constantBufferDirectionalLights;
		std::array<ConstantBufferDataPointLight, sPointLightNum_> constantBufferPointLights;
		std::array<ConstantBufferDataSpotLight, sSpotLightNum_> constantBufferSpotLights;
	};

	std::unique_ptr<ConstantBuffer<ConstantBufferDataLightManager>> constantBufferLightManager_;

public:
	std::array<DirectionalLight, sDirectionalLightNum_> directionalLights;
	std::array<PointLight, sPointLightNum_> pointLights;
	std::array<SpotLight, sSpotLightNum_> spotLights;
	CircleShadow circleShadow;
	Fog fog;

	static bool isPointLighting;
	static bool isSpotLighting;

private:
	void DirectionalLightUpdate();	// ディレクションナルライト
	void PointLightUpdate();		// ポイントライト
	void SpotLightUpdate();			// スポットライト

public:
	void Init();
	void Update();
	void Draw();

private:
	friend Singleton<LightManager>;
	LightManager();
};

