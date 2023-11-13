#pragma once
#include "Weapon.h"
#include "Trajectory.h"

// 剣のクラス
class Sword : public Weapon
{
private:
	std::unique_ptr<Trajectory> mTrajectory;

private:
	void ColliderUpdate() override;
	void CalcDamage() override;

public:
	Sword();
	void Init() override;
	void Update(Transform* parent = nullptr) override;
	void DrawModel() override;

public:
	Vec3 GetTipPos() override;
};

