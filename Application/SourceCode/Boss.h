#pragma once
#include "NewEngine.h"
#include "GaugeParam.h"

class Boss
{
private:
	std::unique_ptr<Object3D> boss_;

	GaugeParam hpGaugeParam_;

	CapsuleCollider collider_;
	CubeCollider bodyCollider_;

private:
	void ColliderUpdate();

public:
	Boss();
	void Init();
	void Update();
	void DrawModel();

public:
	void Damage(const float damage);

public:
	CapsuleCollider GetCollider();
	CubeCollider GetBodyCollider();
	GaugeParam GetHpGaugeParam();
	Vec3 GetPos();

};

