#pragma once
#include "ConstantBuffer.h"
#include "Util.h"

class PointLight
{
private:
	ConstantBuffer<ConstantBufferDataPointLight>* constantBufferPointLight;

public:
	Vec3 pos;
	Color color;
	Vec3 atten;
	bool isActive;

public:
	PointLight();
	~PointLight();
	void Init();
	void Update();
	void Draw();
};

