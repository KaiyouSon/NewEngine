#pragma once
#include "Player.h"
#include "Boss.h"
#include "Field.h"
#include "UIManager.h"
#include "MovieEvent.h"

// 前方宣言
template<typename T> class Singleton;

// 当たり判定を処理するクラス
class CollisionManager : public Singleton<CollisionManager>
{
private:
	Player* mPlayer;
	Boss* mBoss;
	Field* mField;
	UIManager* mUiManager;
	MovieEvent* mMovieEvent;

private:
	// 押し戻しの処理
	void CubeCollisionPlayer(const CubeCollider& cube);
	void CapsuleCollisionPlayer(const CapsuleCollider& capsule);
	void SphereCollisionPlayer(const SphereCollider& sphere);

private:
	void PlayerHitBoss();
	void PlayerHitNegotiation();
	void PlayerHitFieldObject();
	void PlayerHitAirCollider();

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
	void SetMovieEvent(MovieEvent* movieEvent);

private:
	friend Singleton<CollisionManager>;
};

