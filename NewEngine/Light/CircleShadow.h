#pragma once
#include "ConstantBuffer.h"
#include "Util.h"

class CircleShadow
{
private:
	ConstantBuffer<ConstantBufferDataCircleShadow>* constantBufferCircleShadow;

public:
	Vec3 vec;
	Vec3 pos;
	float disCasterLight;
	Vec3 atten;
	Vec2 factorAngleCos;
	bool isActive;

public:
	CircleShadow();
	~CircleShadow();
	void Update();
	void Draw();
};

