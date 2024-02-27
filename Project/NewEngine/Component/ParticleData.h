#pragma once
#include "Component.h"
#include "Vec4.h"

class ParticleMesh;

class ParticleData : public Component
{
private:
	enum class ParameterType
	{
		Float,
		Vec2,
		Vec3,
		Vec4,
		Color,
		Count
	};

	struct Parameter
	{
		Vec4 param;
		ParameterType type;

		Parameter();
	};

private:
	std::vector<Parameter> mParameters;

private:
	void DrawParameterSlider(Parameter& parameter);
	void DrawParameterType(Parameter& parameter);

public:
	ParticleData();
	ParticleData(GameObject* gameObj);
	void Update() override;

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;

private:
	friend ParticleMesh;
};

