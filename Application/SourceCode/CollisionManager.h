#pragma once
#include "Player.h"
#include "Boss.h"

class CollisionManager
{
private:
	Player* player_;
	Boss* boss_;

private:
	void PlayerHitBoss();

public:
	void Update();

public:
	void SetPlayer(Player* player);
	void SetBoss(Boss* boss);
};

