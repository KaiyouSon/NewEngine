#pragma once
#include "NewEngine.h"
#include "Weapon.h"
#include "MoveMotion.h"
#include "BackstepMotion.h"
#include "RollMotion.h"

struct Weapon;
class Player;

enum class PartID
{
	Transform,

	Head,	// 頭
	Body,	// 体

	RightArm,	// 右腕
	RightHand,	// 右手
	LeftArm,	// 左腕
	LeftHand,	// 左手

	RightThigh,	 // 右太もも
	RightLeg,	 // 右足
	LeftThigh,	 // 左太もも
	LeftLeg,	 // 左足

	RightWeapon,
	LeftWeapon,
};

class HumanoidBody
{
private:
	std::array<std::unique_ptr<Object3D>, 11> parts_;
	std::unique_ptr<MoveMotion> moveMotion_;
	std::unique_ptr<BackstepMotion> backstepMotion_;
	std::unique_ptr<RollMotion> rollMotion_;
	std::array<Weapon*, 2> weapons_;
	CapsuleCollider bodyCollider_;
	std::array<Vec3, 11> curRots_;

public:
	Vec3 pos;
	Vec3 attackPos;
	Vec3 rot;
	Vec3 scale;
	Vec3 frontVec;
	Player* parent;

public:
	void ColliderUpdate();

public:
	HumanoidBody();
	void Init();
	void PrevUpdate();
	void PostUpdate();
	void DrawModel();
	void DrawDebugGui();

public:
	void IdleMotion();
	void JoggingMotion();
	void RunMotion();
	void AttackMotion();
	void BackstepMotionUpdate();
	void RollMotionUpdate();

	void CalcFrontVec();
	std::vector<Vec3> CalcCurRots();
	void ChangeMoveMotionInit();

public:
	void SetWeapon(Weapon* weapon, const uint32_t index);

public:
	CapsuleCollider GetBodyCollider();
	Vec3 GetWorldPos(const PartID partID);
	Object3D* GetPart(const PartID partID);
	uint32_t GetPartsSize();
	bool GetisPlayMoveMotion();
	bool GetisPlayBackStepMotion();
	bool GetisPlayRollMotion();
	bool GetisPlayAttackMotion(const uint32_t index);
	bool GetisAttackMotionCanChange(const uint32_t index);
	bool GetisBackStepMotionCanChange();
	bool GetisRollMotionCanChange();
	void BackstepMotionInit();
	void RollMotionInit();
	void AttackMotionInit(const uint32_t index);
};

