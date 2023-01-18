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
	Transform transform;

public:
	//Model model;
	Texture texture;
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;
	Object3D* obj;

public:
	SilhouetteObj();
	~SilhouetteObj();
	void Update(const SilhouetteObj* parent = nullptr);
	void Draw();

	static void SetBlendMode(const BlendMode& blendMode);
};

