#pragma once
#include "GameObject.h"

class EmptyObject : public GameObject
{
private:
	void InitComponents();

public:
	EmptyObject();
	EmptyObject(const std::string& name);
	void Update() override;
	void Copy(GameObject* gameObj) override;

private:
	void ExecuteCS() override {}
	void AppedToRenderer() override {}
	void Draw([[maybe_unused]] const std::string& _layerTag = "", [[maybe_unused]] const BlendMode _blendMode = BlendMode::Alpha) override {}
	void SetTexture([[maybe_unused]] const std::string& textureTag, [[maybe_unused]] const bool isChangeSize = true) override {}
};

