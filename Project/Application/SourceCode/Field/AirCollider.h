#pragma once
#include "NewEngine.h"
#include "IFieldObject.h"

class AirCollider : public IFieldObject
{
private:
	CapsuleCollider mCollider;

public:
	AirCollider();
	void Init() override {};
	void Update() override;
	void ExecuteCS() override {};
	void Draw(const bool isDrawDepth = false) override { isDrawDepth; };

public:
	void SetCollider(const CapsuleCollider& collider);

public:
	CapsuleCollider GetCollider();
};

