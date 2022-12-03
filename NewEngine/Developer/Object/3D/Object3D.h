#pragma once
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Model.h"
#include "Texture.h"
#include "Util.h"

class Object3D
{
private:
	ConstantBuffer* constantBuffer;
	Transform transform;
	bool isInitConstantBuffer;

private:
	void InitConstantBuffer();

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
};

