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
	bool isLighting;

public:
	Model model;
	Texture texture;
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;

public:
	Object3D();
	~Object3D();
	void Update();
	void Draw();

	static void SetBlendMode(const BlendMode& blendMode);
	inline void SetisLighting(const bool& isLighting) { this->isLighting = isLighting; }
};

