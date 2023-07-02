#pragma once
#include "NewEngine.h"
#include "GaugeParam.h"
#include "HumanoidBody.h"

class Player
{
public:
	enum class State
	{
		Idle,
		Jogging,
		AttackR1,
	};

private:
	std::array<GaugeParam, 3> gaugePrames_;
	std::unique_ptr<Weapon> weapon_;

	std::unique_ptr<HumanoidBody> player_;
	Vec3 frontVec;

	State state_;

	float moveSpeed;

private:
	void GaugeParamInit();

private:
	void MoveUpdate();

	void IdleUpdate();
	void JoggingUpdate();
	void RunUpdate();
	void AttackR1Update();

public:
	Player();
	void Init();
	void Update();
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
	Vec3 GetHeadPos();
	State GetState();
};

