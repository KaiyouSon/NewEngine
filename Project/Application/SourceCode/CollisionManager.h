#pragma once
#include "Player.h"
#include "Boss.h"
#include "Field.h"
#include "UIManager.h"

template<typename T> class Singleton;

class CollisionManager : public Singleton<CollisionManager>
{
private:
	Player* player_;
	Boss* boss_;
	Field* field_;
	UIManager* uiManager_;

private:
	void PlayerHitBoss();
	void PlayerHitMessageSign();

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

