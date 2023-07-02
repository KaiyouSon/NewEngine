#pragma once
#include "NewEngine.h"
#include "Weapon.h"

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
	std::array<Weapon*, 2> weapons_;

private:
	bool isReverce;
	Easing joggingEase;

private:
	Easing attackEase_;
	uint32_t step_;
	bool isPlay_;
	std::array<Vec3, 11> curRots_;

private:
	void CalcCurrentRot();

public:
	Vec3 pos;
	Vec3 rot;

public:
	HumanoidBody();
	void Init();
	void Update();
	void DrawModel();
	void DrawDebugGui();

public:
	void IdleMotion();
	void JoggingMotion();
	void AttackMotion();

public:
	void SetWeapon(Weapon* weapon, const uint32_t index);

public:
	Vec3 GetWorldPos(const PartID partID);
	Object3D* GetPart(const PartID partID);
	uint32_t GetPartsSize();
};

