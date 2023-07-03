#pragma once
#include "NewEngine.h"
#include "Weapon.h"
#include "JoggingMotion.h"

struct Weapon;

enum class PartID
{
	Transform,

	Head,	// ��
	Body,	// ��

	RightArm,	// �E�r
	RightHand,	// �E��
	LeftArm,	// ���r
	LeftHand,	// ����

	RightThigh,	 // �E������
	RightLeg,	 // �E��
	LeftThigh,	 // ��������
	LeftLeg,	 // ����

	RightWeapon,
	LeftWeapon,
};

class HumanoidBody
{
private:
	std::array<std::unique_ptr<Object3D>, 11> parts_;
	std::unique_ptr<JoggingMotion> joggingMotion_;
	std::array<Weapon*, 2> weapons_;
	CapsuleCollider bodyCollider_;

private:
	bool isReverce_;
	Easing joggingEase_;
	Easing runEase_;

public:
	Vec3 pos;
	Vec3 attackPos;
	Vec3 rot;
	Vec3 scale;
	Vec3 frontVec;
	Vec3 vel;

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
	void JoggingMotionUpdate();
	void RunMotion();
	void AttackMotion();

	void CalcFrontVec();

public:
	void SetWeapon(Weapon* weapon, const uint32_t index);

public:
	CapsuleCollider GetBodyCollider();
	Vec3 GetWorldPos(const PartID partID);
	Object3D* GetPart(const PartID partID);
	uint32_t GetPartsSize();
	bool GetisPlayAttackMotion(const uint32_t index);
};

