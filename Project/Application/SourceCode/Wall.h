#pragma once
#include "NewEngine.h"

class Wall
{
private:
	Transform mParent;
	std::unique_ptr<Object3D> mWall;
	SphereCollider mCollider;

public:
	Wall();
	void Init();
	void Update();
	void DrawModel();

public:
	void SetParent(const Transform parent);
	void SetCollider(const SphereCollider collider);
	void SetModel(Model* model);

public:
	Vec3 GetPos();
	SphereCollider GetCollider();
};

