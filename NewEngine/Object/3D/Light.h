#pragma once
#include "Vec3.h"
#include "ConstantBuffer.h"

class Light
{
private:
	ConstantBuffer<ConstantBufferDataLight>* constantBufferLight;

public:
	Vec3 lightDir;
	Vec3 lightColor;

	static Light current;

public:
	Light();
	~Light();

	void Init();
	void Update();
	void Draw();
};

