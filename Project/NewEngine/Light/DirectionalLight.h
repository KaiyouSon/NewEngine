#pragma once
#include "ILight.h"
#include "Vec3.h"
#include "Color.h"

// 平行光源のクラス
class DirectionalLight : public ILight
{
public:
	Vec3 pos;
	Color color;
	bool isActive;

public:
	DirectionalLight();
};