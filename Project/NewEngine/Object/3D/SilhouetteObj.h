#pragma once
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Model.h"
#include "Texture.h"
#include "Util.h"
#include "Object3D.h"

// 最近使ってないためリファクタリングは後回し
class SilhouetteObj
{
private:
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CTransform3D>> mConstantBufferTransform;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CColor>> mConstantBufferColor;
	GraphicsPipeline* mGraphicsPipeline;
	Transform mTransform;
	Transform* mParent;

public:
	Object3D* obj;
	Color color;

public:
	SilhouetteObj();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode& blendMode = BlendMode::Alpha);

	void SetBlendMode(const BlendMode& blendMode);
};

