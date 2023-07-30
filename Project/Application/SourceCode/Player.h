#pragma once
#include "NewEngine.h"
#include "GaugeParam.h"
#include "HumanoidBody.h"
#include "PlayerBody.h"

class MalletMotion;

class Player : public Character
{
public:
	enum class State
	{
		Idle,
		Jogging,
		Run,
		Backstep,
		Roll,
		AttackR1,
		AttackR2,
		AttackBack,
		AttackRoll,
		Drink,
	};

private:
	std::array<GaugeParam, 3> gaugePrames_;
	std::unique_ptr<Weapon> weapon_;
	std::unique_ptr<PlayerBody> player_;

	bool isAlive_;
	bool isDissolve;

private:
	Timer pushTimer; //ボタン連打させないように

private:
	// 状態
	State state_;

private:
	// 移動関連
	float moveSpeed_;
	float joggingSpeed_;
	float runSpeed_;
	Vec3 moveVel;

private:
	// 当たりあ判定関連
	Vec3 frontVec_;
	CapsuleCollider bodyCollider_;

private:
	// ダメージ関連
	bool isDamage_;
	Timer damageCoolTimer_;

private:
	void CalcFrontVec();
	void CalcBodyCollider();
	void ColliderUpdate();

private:
	void GaugeParamInit();
	void GaugeParamUpdate();

private:
	void MoveUpdate();

	void IdleUpdate();
	void JoggingUpdate();
	void RunUpdate();
	void BackstepUpdate();
	void RollUpdate();
	void AttackR1Update();
	void AttackR2Update();
	void AttackBackUpdate();
	void AttackRollUpdate();
	void DrinkUpdate();

private:
	void DamageUpdate();

public:
	Player();
	void Init();
	void PrevUpdate();
	void PostUpdate();
	void DrawModel();
	void DrawDebugGui();

public:
	void Recovery();
	void Damage(const float damage);

public:
	void SetPos(const Vec3 pos);
	void SetisDamage(const bool isDamage);

public:
	GaugeParam GetGaugeParam(const uint32_t index);
	PlayerBody* GetPlayerBody();
	Weapon* GetWeapon();
	Vec3 GetPos();
	Vec3 GetRot();
	Vec3 GetAttackPos();
	Vec3 GetHeadPos();
	Vec3 GetMoveVel();
	Vec3 GetFrontVec();
	State GetState();
	CapsuleCollider GetBodyCollider();
	bool GetisDamage();
	bool GetisAlive();
	bool GetisDissolve();

private:
	friend PlayerBody;
	friend MoveMotion;
	friend MalletMotion;
	friend BackstepMotion;
	friend RollMotion;
};

