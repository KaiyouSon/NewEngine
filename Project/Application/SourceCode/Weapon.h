#pragma once
#include "IWeaponMotion.h"

struct Weapon
{
protected:
	Vec3 localPos_;
	Vec3 localRot_;
	bool isCalcCollider_;

protected:
	float weakAttackDamage_;
	float heavyAttackDamage_;
	float backAttackDamage_;
	float rollAttackDamage_;
	float damage_;

public:
	std::unique_ptr<Object3D> weapon;
	std::unique_ptr<IWeaponMotion> motion;
	CapsuleCollider collider;

private:
	virtual void CalcDamage() = 0;

public:
	virtual ~Weapon() {}
	virtual void Init() = 0;
	virtual void Update(Transform* parent = nullptr) = 0;
	virtual void DrawModel() = 0;

public:
	virtual void ColliderUpdate() = 0;

public:
	float GetDamage();

public:
	void SetisCalcCollider(const bool isCalcCollide);
};

