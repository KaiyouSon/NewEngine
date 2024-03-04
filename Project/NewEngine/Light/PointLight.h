#pragma once
#include "GameObject.h"

class PointLight : public GameObject
{
public:
	Vec3 colorRate;
	float radius;
	float decay;

private:
	void InitComponents();

public:
	PointLight();
	PointLight(const std::string& name);
	~PointLight();
	void Update() override;

private:
	//使わない関数群
	void ExecuteCS() override {};
	void AppedToRenderer() override {};
	void Draw(const std::string& _layerTag = "", const BlendMode _blendMode = BlendMode::Alpha) override { _layerTag; _blendMode; };
	void SetTexture(const std::string& textureTag, [[maybe_unused]] const bool isChangeSize = true) override { textureTag; };
};