#pragma once
#include "NewEngine.h"
#include "GaugeParam.h"
#include "BossBody.h"

class Player;
class BossGrabAttackMotion;
class BossAttack2Motion;
class BossAttack3Motion;

class Boss : public Character
{
private:
	std::unique_ptr<BossBody> boss_;
	std::unique_ptr<Weapon> weapon;
	Player* player_;

	GaugeParam hpGaugeParam_;

	CapsuleCollider collider_;
	CubeCollider bodyCollider_;

	Vec3 frontVec_;
	float rotY_;

private:
	// ダメージ関連
	bool isDamage_;
	Timer damageCoolTimer_;

private:
	// モーション切り替え関連
	Timer coolTimer_;
	uint32_t motionNum_;

private:
	void CalcFrontVec();
	void ColliderUpdate();

	void MotionUpdate();

public:
	Boss();
	void Init();
	void Update();
	void DrawModel();

public:
	void Damage(const float damage);

public:
	void SetPlayer(Player* player);
	void SetisDamage(const bool isDamage);

public:
	CapsuleCollider GetCollider();
	CubeCollider GetBodyCollider();
	GaugeParam GetHpGaugeParam();
	Vec3 GetPos();
	bool GetisDamage();
	Weapon* GetWeapon();

private:
	friend BossGrabAttackMotion;
	friend BossAttack2Motion;
	friend BossAttack3Motion;

};

