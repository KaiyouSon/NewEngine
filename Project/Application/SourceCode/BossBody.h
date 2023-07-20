#pragma once
#include "HumanoidBody.h"
#include "BossAttack2Motion.h"

class Boss;

class BossBody : public HumanoidBody
{
private:
	std::unique_ptr<BossAttack2Motion> attack2Motion_;

public:
	Boss* parent;

public:
	BossBody();
	void Init();
	void Update();
	void DrawModel();
};

