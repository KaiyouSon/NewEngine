#pragma once
#include "Enum.h"

class ICollider
{
protected:
	ColliderPrimitive primitive_;
	bool isActive = true;

public:
	virtual ~ICollider() {}

};

