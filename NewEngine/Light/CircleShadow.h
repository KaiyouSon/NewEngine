#pragma once
#include "ConstantBuffer.h"
#include "Util.h"
#include <memory>

class CircleShadow
{
private:
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CCircleShadow>> constantBufferCircleShadow_;

public:
	Vec3 vec;
	Vec3 pos;
	float disCasterLight;
	Vec3 atten;
	Vec2 factorAngleCos;
	bool isActive;

public:
	CircleShadow();
	void Update();
	void Draw();
};

