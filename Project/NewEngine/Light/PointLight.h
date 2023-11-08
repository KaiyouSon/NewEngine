#pragma once
#include "ILight.h"
#include "Vec3.h"
#include "Color.h"

class PointLight : public ILight
{
public:
	Vec3 pos;
	Vec3 colorRate;
	Color color;
	float radius;
	float decay;
	bool isActive;

public:
	PointLight();
};