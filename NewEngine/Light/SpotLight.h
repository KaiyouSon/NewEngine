#pragma once
#include "ConstantBuffer.h"
#include "Util.h"

class SpotLight
{
private:
	ConstantBuffer<ConstantBufferDataSpotLight>* constantBufferSpotLight;

public:
	Vec3 vec;
	Vec3 pos;
	Vec3 color;
	Vec3 atten;
	Vec2 factorAngleCos;
	unsigned int isActive;

public:
	SpotLight();
	~SpotLight();
	void Init();
	void Update();
	void Draw();
};

