#pragma once
#include "Player.h"
#include "Boss.h"

template<typename T> class Singleton;

class CollisionManager : public Singleton<CollisionManager>
{
private:
	Player* player_;
	Boss* boss_;

private:
	void PlayerHitBoss();

public:
	void Update();
	bool IsCheckFrontBoss(const Vec3 pos, const Vec3 front);
	bool IsCheckPlayerMove(float* dis);

public:
	void SetPlayer(Player* player);
	void SetBoss(Boss* boss);

private:
	friend Singleton<CollisionManager>;
};

