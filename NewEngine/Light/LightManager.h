#pragma once
#include "CircleShadow.h"
#include <array>

struct DirectionalLight
{
	Vec3 dirVec = Vec3::up;
	Color color = Color::white;
	bool isActive = true;

	DirectionalLight() {}
	~DirectionalLight() {}
};

struct PointLight
{
	Vec3 pos = Vec3::up;
	Color color = Color::white;
	Vec3 atten = { 0.3f,0.1f,0.1f };
	bool isActive = true;

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
	bool isActive = true;

	SpotLight() {}
	~SpotLight() {}
};

template<typename T> class Singleton;
class LightManager : public Singleton<LightManager>
{
private:
	static const int directionalLightNum = 1;
	static const int pointLightNum = 3;
	static const int spotLightNum = 3;

	struct ConstantBufferDataLightManager
	{
		std::array<ConstantBufferDataDirectionalLight, directionalLightNum> constantBufferDirectionalLights;
		std::array<ConstantBufferDataPointLight, pointLightNum> constantBufferPointLights;
		std::array<ConstantBufferDataSpotLight, spotLightNum> constantBufferSpotLights;
	};

	ConstantBuffer<ConstantBufferDataLightManager>* constantBufferLightManager;

public:
	std::array<DirectionalLight, directionalLightNum> directionalLights;
	std::array<PointLight, pointLightNum> pointLights;
	std::array<SpotLight, spotLightNum> spotLights;
	CircleShadow circleShadow;

private:
	friend Singleton<LightManager>;
	LightManager();

public:
	~LightManager();
	void Init();
	void Update();
	void Draw();
};

