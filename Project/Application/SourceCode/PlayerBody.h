#pragma once
#include "HumanoidBody.h"
#include "MoveMotion.h"
#include "BackstepMotion.h"
#include "RollMotion.h"

struct Weapon;
class Player;

class PlayerBody
{
private:
	std::array<std::unique_ptr<Object3D>, 11> parts_;
	std::array<Weapon*, 2> weapons_;
	std::array<Vec3, 13> curRots_;

private:
	std::unique_ptr<MoveMotion> moveMotion_;
	std::unique_ptr<RollMotion> rollMotion_;
	std::unique_ptr<BackstepMotion> backstepMotion_;

public:
	Vec3 pos;
	Vec3 attackPos;
	Vec3 rot;
	Vec3 scale;
	Player* parent;

public:
	PlayerBody();
	void Init();
	void PrevUpdate();
	void PostUpdate();
	void DrawModel();
	void DrawDebugGui();

public:
	void IdleMotion();
	void JoggingMotion();
	void RunMotion();
	void AttackR1MotionUpdate();
	void AttackR2MotionUpdate();
	void AttackBackMotionUpdate();
	void AttackRollMotionUpdate();
	void BackstepMotionUpdate();
	void RollMotionUpdate();

	void CalcFrontVec();
	std::vector<Vec3> CalcCurRots();
	std::vector<Vec3> CalcCurWeaponRots();
	void ChangeMoveMotionInit();

public:
	void SetWeapon(Weapon* weapon, const WeaponPartID partID);

public:
	CapsuleCollider GetBodyCollider();
	Vec3 GetWorldPos(const PartID partID);
	Object3D* GetPart(const PartID partID);
	Object3D* GetWeaponPart(const WeaponPartID partID);
	uint32_t GetPartsSize();
	uint32_t GetWeaponPartsSize();
	bool GetisPlayMoveMotion();
	bool GetisPlayBackStepMotion();
	bool GetisPlayRollMotion();
	bool GetisPlayAttackMotion(const WeaponPartID partID);
	bool GetisAttackMotionCanChange(const WeaponPartID partID);
	bool GetisBackStepMotionCanChange();
	bool GetisRollMotionCanChange();
	void BackstepMotionInit();
	void RollMotionInit();
	void AttackMotionInit(const WeaponPartID partID);
};

