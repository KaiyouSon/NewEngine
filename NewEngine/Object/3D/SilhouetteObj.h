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
	ConstantBuffer<ConstantBufferDataTransform3D>* constantBufferTransform;
	ConstantBuffer<ConstantBufferDataColor>* constantBufferColor;
	GraphicsPipeline* graphicsPipeline;
	Transform transform;

public:
	Object3D* obj;
	Color color;

public:
	SilhouetteObj();
	~SilhouetteObj();
	void Update(const SilhouetteObj* parent = nullptr);
	void Draw(const BlendMode& blendMode = BlendMode::Alpha);

	void SetBlendMode(const BlendMode& blendMode);
};

