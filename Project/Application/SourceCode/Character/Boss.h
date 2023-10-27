#pragma once
#include "NewEngine.h"
#include "GaugeParam.h"
#include "BossBody.h"

// 前方宣言
class Player;
class BossAttack2Motion;
class BossAttack3Motion;

// ボスのクラス
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
	// damage関連
	bool mIsDamage;
	Timer mDamageCoolTimer;

private:
	// モーション関連
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
	friend BossAttack2Motion;
	friend BossAttack3Motion;
};
