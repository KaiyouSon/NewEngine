#pragma once
#include "ILight.h"
#include "Vec3.h"
#include "Color.h"

class SpotLight : public GameObject
{
public:
	Vec3 vec;
	Vec3 colorRate;
	Vec2 cosAngle;
	float radius;	// ライトの届く最大距離
	float decay;	// 減衰率

private:
	void InitComponents();

public:
	SpotLight();
	SpotLight(const std::string& name);
	~SpotLight();
	void Update() override;

private:
	//使わない関数群
	void ExecuteCS() override {};
	void AppedToRenderer() override {};
	void Draw(const std::string& _layerTag = "", const BlendMode _blendMode = BlendMode::Alpha) override { _layerTag; _blendMode; };
	void SetTexture(const std::string& textureTag, [[maybe_unused]] const bool isChangeSize = true) override { textureTag; };
};