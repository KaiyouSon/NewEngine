#pragma once
#include "Player.h"
#include "Boss.h"
#include "Field.h"
#include "UIManager.h"

template<typename T> class Singleton;

class CollisionManager : public Singleton<CollisionManager>
{
private:
	Player* mPlayer;
	Boss* mBoss;
	Field* mField;
	UIManager* mUiManager;

private:
	void PlayerHitBoss();
	void PlayerHitMessageSign();
	void PlayerHitRespawnPoint();
	void PlayerHitNegotiation();

	void BossHitPlayer();

public:
	void Update();

	float CalcPlayerDisToFront(const Vec3 frontVec, const float max);
	bool IsCheckFrontBoss(const Vec3 pos, const Vec3 front);

public:
	void SetPlayer(Player* player);
	void SetBoss(Boss* boss);
	void SetField(Field* field);
	void SetUIManager(UIManager* uiManager);

private:
	friend Singleton<CollisionManager>;
};

