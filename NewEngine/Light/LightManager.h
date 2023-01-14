#pragma once
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Singleton.h"
#include <array>

template<typename T> class Singleton;

class LightManager : public Singleton<LightManager>
{
private:
	friend Singleton<LightManager>;
	static const int directionalLightNum;
	static const int pointLightNum;

public:
	std::array<DirectionalLight, 3> directionalLights;
	std::array<PointLight, 3> pointLights;

	void Init();
	void Update();
	void Draw();
};

