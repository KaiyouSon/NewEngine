#pragma once
#include "NewEngine.h"
#include "GaugeParam.h"
#include "BossBody.h"

class Player;
class BossGrabAttackMotion;
class BossAttack2Motion;
class BossAttack3Motion;

class Boss : public Character
{
private:
	std::unique_ptr<BossBody> mBoss;
	std::unique_ptr<Weapon> mWeapon;
	Player* mPlayer;
	bool mIsAlive;
	bool mIsDissolve;
	bool mIsFight;

	GaugeParam mHpGaugeParam;

	CapsuleCollider mCollider;
	CubeCollider mBodyCollider;

	Vec3 mFrontVec;
	float mRotY;

	float mDamage;

private:
	// 繝繝｡繝ｼ繧ｸ髢｢騾｣
	bool mIsDamage;
	Timer mDamageCoolTimer;

private:
	// 繝｢繝ｼ繧ｷ繝ｧ繝ｳ蛻・ｊ譖ｿ縺磯未騾｣
	Timer mCoolTimer;
	uint32_t mMotionNum;

private:
	void CalcFrontVec();
	void ColliderUpdate();

	void MotionUpdate();

public:
	Boss();
	void Init();
	void Update();
	void DrawModel();

public:
	void Damage(const float damage);

public:
	void SetPlayer(Player* player);
	void SetisDamage(const bool isDamage);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

public:
	CapsuleCollider GetCollider();
	CubeCollider GetBodyCollider();
	GaugeParam GetHpGaugeParam();
	Vec3 GetPos();
	bool GetisDamage();
	Weapon* GetWeapon();
	float GetDamage();
	bool GetisAlive();
	bool GetisDissolve();
	bool GetisFight();

private:
	friend BossGrabAttackMotion;
	friend BossAttack2Motion;
	friend BossAttack3Motion;
};
