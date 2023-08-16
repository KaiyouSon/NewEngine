#pragma once
#include "NewEngine.h"

class Coffin
{
private:
	Transform mParent;
	std::unique_ptr<Object3D> mCoffinTop;
	std::unique_ptr<Object3D> mCoffinBottom;
	CubeCollider mBottomCollider;

public:
	Coffin();
	void Init();
	void Update();
	void DrawModel();

public:
	void SetParent(const Transform transform);
	void SetTopPos(const Vec3 pos);
	void SetBottomPos(const Vec3 pos);
	void SetTopRot(const Vec3 rot);
	void SetBottomRot(const Vec3 rot);
	void SetBottomCollider(const CubeCollider& collider);

public:
	CubeCollider GetBottomCollider();
	Vec3 GetPos();
};