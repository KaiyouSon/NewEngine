#pragma once
#include "GameObject.h"

// 平行光源のクラス
class DirectionalLight : public GameObject
{
private:
	void InitComponents();

public:
	DirectionalLight();
	DirectionalLight(const std::string& name);
	~DirectionalLight();
	void Update() override;

private:
	//使わない関数群
	void ExecuteCS() override {};
	void AppedToRenderer() override {};
	void Draw(const std::string& _layerTag = "", const BlendMode _blendMode = BlendMode::Alpha) override { _layerTag; _blendMode; };
	void SetTexture(const std::string& textureTag, [[maybe_unused]] const bool isChangeSize = true) override { textureTag; };
};