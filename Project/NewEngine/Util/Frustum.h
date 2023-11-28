#pragma once
#include "NewEngineEnum.h"
#include <array>

struct Frustum
{
	std::array<Vec3, 6> planes;
};

