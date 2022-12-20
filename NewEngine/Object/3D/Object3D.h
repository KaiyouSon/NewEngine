#pragma once
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Model.h"
#include "Texture.h"
#include "Util.h"

class Object3D
{
private:
	ConstantBuffer<ConstantBufferDataTransform3D>* constantBufferTransform;
	ConstantBuffer<ConstantBufferDataMaterial>* constantBufferMaterial;
	ConstantBuffer<ConstantBufferDataColor>* constantBufferColor;
	Transform transform;

public:
	Model model;
	Texture texture;
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;
	bool isLighting;

public:
	Object3D();
	~Object3D();
	void Update(const Object3D* parent = nullptr);
	void Draw();

	static void SetBlendMode(const BlendMode& blendMode);
};

