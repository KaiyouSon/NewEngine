#pragma once
#include "NewEngineEnum.h"

struct ICollider
{
public:
	ColliderPrimitive primitive;
	bool isActive = true;

public:
	virtual ~ICollider() {}
};

