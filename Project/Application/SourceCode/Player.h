#pragma once
#include "NewEngine.h"
#include "GaugeParam.h"
#include "HumanoidBody.h"

class MalletMotion;

class Player
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
	};

private:
	std::array<GaugeParam, 3> gaugePrames_;
	std::unique_ptr<Weapon> weapon_;

	std::unique_ptr<HumanoidBody> player_;

	Timer pushTimer; //ƒ{ƒ^ƒ“˜A‘Å‚³‚¹‚È‚¢‚æ‚¤‚É

	State state_;

	float moveSpeed_;
	float joggingSpeed_;
	float runSpeed_;
	Vec3 moveVel;

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

public:
	Player();
	void Init();
	void PrevUpdate();
	void PostUpdate();
	void DrawModel();
	void DrawDebugGui();

	void CalcBodyCollider();

public:
	void SetPos(const Vec3 pos);

public:
	GaugeParam GetGaugeParam(const uint32_t index);
	HumanoidBody* GetHumanoidBody();
	Weapon* GetWeapon();
	Vec3 GetPos();
	Vec3 GetAttackPos();
	Vec3 GetHeadPos();
	Vec3 GetMoveVel();
	Vec3 GetFrontVec();
	State GetState();

	friend HumanoidBody;
	friend MoveMotion;
	friend MalletMotion;
	friend BackstepMotion;
	friend RollMotion;
};

