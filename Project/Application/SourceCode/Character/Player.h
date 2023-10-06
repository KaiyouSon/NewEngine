#pragma once
#include "NewEngine.h"
#include "GaugeParam.h"
#include "HumanoidBody.h"
#include "PlayerBody.h"
#include "MovieEvent.h"

class MalletMotion;

class Player : public Character
{
public:
	enum class State
	{
		Idle,
		Jogging,
		Run,
		Backstep,
		Roll,
		AttackR1,
		AttackR2,
		AttackBack,
		AttackRoll,
		Drink,
		OpenGate,
	};

private:
	std::array<GaugeParam, 3> mGaugePrames;
	std::unique_ptr<Weapon> mWeapon;
	std::unique_ptr<PlayerBody> mPlayer;
	uint32_t mBottleNum;

	bool mIsAlive;
	bool mIsDissolve;

private:
	Timer mPushTimer; //繝懊ち繝ｳ騾｣謇薙＆縺帙↑縺・ｈ縺・↓

private:
	// 迥ｶ諷・
	State mState;

private:
	// 遘ｻ蜍暮未騾｣
	float mMoveSpeed;
	float mJoggingSpeed;
	float mRunSpeed;
	Vec3 mMoveVel;

private:
	// 蠖薙◆繧翫≠蛻､螳夐未騾｣
	Vec3 mFrontVec;
	CapsuleCollider mBodyCollider;

private:
	// 繝繝｡繝ｼ繧ｸ髢｢騾｣
	bool mIsDamage;
	Timer mDamageCoolTimer;

private:
	MovieEvent* mMovieEvent;

public:
	void CalcFrontVec();
private:
	void CalcBodyCollider();
	void ColliderUpdate();

private:
	void GaugeParamInit();
	void GaugeParamUpdate();

private:
	void MoveUpdate();

	void IdleUpdate();
	void JoggingUpdate();
	void RunUpdate();
	void BackstepUpdate();
	void RollUpdate();
	void AttackR1Update();
	void AttackR2Update();
	void AttackBackUpdate();
	void AttackRollUpdate();
	void DrinkUpdate();
	void OpenGateUpdate();

private:
	void DamageUpdate();

public:
	Player();
	void Init();
	void PrevUpdate();
	void PostUpdate();
	void DrawModel();
	void DrawDebugGui();

public:
	void Recovery();
	void Damage(const float damage);

public:
	void SetMovieEvent(MovieEvent* movieEvent);
	void SetPos(const Vec3 pos);
	void SetRot(const Vec3 rot);
	void SetisDamage(const bool isDamage);
	void SetState(const State state);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

public:
	GaugeParam GetGaugeParam(const uint32_t index);
	PlayerBody* GetPlayerBody();
	Weapon* GetWeapon();
	Vec3 GetPos();
	Vec3 GetRot();
	Vec3 GetAttackPos();
	Vec3 GetHeadPos();
	Vec3 GetMoveVel();
	Vec3 GetFrontVec();
	State GetState();
	CapsuleCollider GetBodyCollider();
	bool GetisDamage();
	bool GetisAlive();
	bool GetisDissolve();
	uint32_t GetBottleNum();

private:
	friend PlayerBody;
	friend MoveMotion;
	friend MalletMotion;
	friend BackstepMotion;
	friend RollMotion;
};
