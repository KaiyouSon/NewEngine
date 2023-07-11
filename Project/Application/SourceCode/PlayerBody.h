#pragma once
#include "HumanoidBody.h"
#include "MoveMotion.h"
#include "BackstepMotion.h"
#include "RollMotion.h"

struct Weapon;
class Player;

class PlayerBody : public HumanoidBody
{
private:
	std::unique_ptr<MoveMotion> moveMotion_;
	std::unique_ptr<RollMotion> rollMotion_;
	std::unique_ptr<BackstepMotion> backstepMotion_;

public:
	Player* parent;

public:
	PlayerBody();
	void Init();
	void DebugUpdate();
	void Update();
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

	void ChangeMoveMotionInit();

public:
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

