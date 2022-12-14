#pragma once
#include "Vec3.h"
#include "ConstantBuffer.h"

class Light
{
private:
	ConstantBuffer<ConstantBufferDataLight>* constantBufferLight;
	static Light* current;

public:
	Vec3 lightDir = { 1,0,0 };
	Vec3 lightColor = { 1,1,1 };
	bool isDirty = false;


public:
	Light();
	~Light();

	void Init();
	void Update();
	void Draw();

	static Light* GetCurrent() { return current; }
	static void SetCurrent(Light& light) { current = &light; }
};

