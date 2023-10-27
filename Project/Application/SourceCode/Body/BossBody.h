#pragma once
#include "HumanoidBody.h"
#include "BossAttack2Motion.h"
#include "BossAttack3Motion.h"

class Boss;

class BossBody : public HumanoidBody
{
private:
	std::unique_ptr<BossAttack2Motion> mAttack2Motion;
	std::unique_ptr<BossAttack3Motion> mAttack3Motion;
	bool mIsPlayMotion;

public:
	Boss* parent;

	void Attack2Motion();
	void Attack3Motion();

public:
	BossBody();
	void Init();
	void Update();
	void DrawModel();

public:
	bool GetisPlayMotion();

private:
	friend Boss;
};

