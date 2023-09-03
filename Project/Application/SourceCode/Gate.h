#pragma once
#include "NewEngine.h"

class Gate
{
private:
	std::unique_ptr<Object3D> mGateLeft;
	std::unique_ptr<Object3D> mGateRight;
	CapsuleCollider mLeftCollider;
	CapsuleCollider mRightCollider;
	Vec3 centerPos;

public:
	Gate();
	void Init();
	void Update();
	void DrawModel();

public:
	void SetLeftTransform(const Transform& transform);
	void SetRightTransform(const Transform& transform);
	void SetLeftCollider(const CapsuleCollider collider);
	void SetRightCollider(const CapsuleCollider collider);
	void SetCenterPos(const Vec3 pos);
	void SetLeftRot(const Vec3 rot);
	void SetRightRot(const Vec3 rot);

public:
	CapsuleCollider GetLeftCollider();
	CapsuleCollider GetRightCollider();
	Vec3 GetCenterPos();
};
