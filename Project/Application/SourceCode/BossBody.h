#pragma once
#include "HumanoidBody.h"
#include "BossGrabAttackMotion.h"
#include "BossAttack2Motion.h"
#include "BossAttack3Motion.h"

class Boss;

class BossBody : public HumanoidBody
{
private:
	std::unique_ptr<BossGrabAttackMotion> grabAttackMotion_;
	std::unique_ptr<BossAttack2Motion> attack2Motion_;
	std::unique_ptr<BossAttack3Motion> attack3Motion_;

public:
	Boss* parent;

public:
	BossBody();
	void Init();
	void Update();
	void DrawModel();
};

