#pragma once
#include "NewEngineEnum.h"

// コライダーのインターフェース
struct ICollider
{
public:
	ColliderPrimitive primitive;
	bool isActive = true;

public:
	virtual ~ICollider() {}
};

