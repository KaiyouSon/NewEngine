#pragma once
#include "Vec3.h"
#include "ConstantBuffer.h"

class Light
{
private:
	ConstantBuffer<ConstantBufferDataLight>* constantBufferLight;

public:
	Vec3 lightPos = { 1,0,0 };
	Vec3 lightColor = { 1,1,1 };
	bool isDirty = false;
	static Light current;

public:
	Light();
	~Light();

	void Init();
	void Update();
	void Draw();
};

