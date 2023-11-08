#pragma once
#include "ILight.h"
#include "Vec3.h"
#include "Color.h"

class SpotLight : public ILight
{
public:
	Vec3 vec;
	Vec3 pos;
	Color color;
	Vec3 colorRate;
	Vec2 cosAngle;
	bool isActive;
	float radius;	// ライトの届く最大距離
	float decay;	// 減衰率

public:
	SpotLight();
};

