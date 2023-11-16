#pragma once
#include "NewEngine.h"

class AirCollider
{
private:
	CapsuleCollider mCollider;

public:
	void Update();

public:
	void SetCollider(const CapsuleCollider& collider);

public:
	CapsuleCollider GetCollider();

};

