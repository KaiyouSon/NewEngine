#pragma once
#include "Material.h"
#include <array>

struct DirectionalLight
{
	Vec3 pos = Vec3::up;
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
	Material material_;

public:
	DirectionalLight directionalLight;

public:
	void Init();
	void Update();
	void DrawCommand(const uint32_t index);

private:
	friend Singleton<LightManager>;
	LightManager();
};

