#pragma once
#include "IWeaponMotion.h"

// 武器の基底クラス
class Weapon
{
protected:
	// 軌跡関連
	bool mIsActiveTrajectory;

protected:
	Vec3 mLocalPos;
	Vec3 mLocalRot;
	bool mIsCalcCollider;

protected:
	float mWeakAttackDamage;
	float mHeavyAttackDamage;
	float mBackAttackDamage;
	float mRollAttackDamage;
	float mDamage;
	float mChargeRate;

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
	virtual Vec3 GetTipPos() = 0; // 先端の座標

public:
	float GetDamage();
	float GetChargeRate();

public:
	void SetisCalcCollider(const bool isCalcCollide);
	void SetChargeRate(const float chargeRate);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
	void SetisActiveTrajectory(const bool isActiveTrajectory);

};