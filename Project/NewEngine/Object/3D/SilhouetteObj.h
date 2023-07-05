#pragma once
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Model.h"
#include "Texture.h"
#include "Util.h"
#include "Object3D.h"

class SilhouetteObj
{
private:
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CTransform3D>> constantBufferTransform_;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CColor>> constantBufferColor_;
	GraphicsPipeline* graphicsPipeline_;
	Transform transform_;
	Transform* parent_;

public:
	Object3D* obj;
	Color color;

public:
	SilhouetteObj();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode& blendMode = BlendMode::Alpha);

	void SetBlendMode(const BlendMode& blendMode);
};

