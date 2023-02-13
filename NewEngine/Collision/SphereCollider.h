#pragma once
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "Transform.h"
#include "Model.h"
#include "Util.h"

class SphereCollider
{
private:
	ConstantBuffer<ConstantBufferDataTransform3D>* constantBufferTransform;
	ConstantBuffer<ConstantBufferDataColor>* constantBufferColor;
	Transform transform;
	Model model;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;

	Vec3 centerPos;
	float radius;
	Color color;

private:
	void VertexInit();

public:
	SphereCollider();
	SphereCollider(const Vec3& centerPos, const float& radius);
	~SphereCollider();
	void Update();
	void Draw();
};

