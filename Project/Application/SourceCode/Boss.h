#pragma once
#include "NewEngine.h"
#include "GaugeParam.h"
#include "BossBody.h"

class Player;
class BossAttack2Motion;

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
	void CalcFrontVec();
	void CalcRotY();
	void ColliderUpdate();

public:
	Boss();
	void Init();
	void Update();
	void DrawModel();

public:
	void Damage(const float damage);

public:
	void SetPlayer(Player* player);

public:
	CapsuleCollider GetCollider();
	CubeCollider GetBodyCollider();
	GaugeParam GetHpGaugeParam();
	Vec3 GetPos();

private:
	friend BossAttack2Motion;

};

