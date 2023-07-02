#pragma once
#include "NewEngine.h"

struct Weapon
{
protected:
	Vec3 localPos_;
	Vec3 localRot_;

public:
	std::unique_ptr<Object3D> weapon;

public:
	virtual ~Weapon() {}
	virtual void Init() = 0;
	virtual void Update(Transform* parent = nullptr) = 0;
	virtual void DrawModel() = 0;
};

