#pragma once
#include "Enum.h"

struct ICollider
{
public:
	ColliderPrimitive primitive;
	bool isActive = true;

public:
	virtual ~ICollider() {}
};

