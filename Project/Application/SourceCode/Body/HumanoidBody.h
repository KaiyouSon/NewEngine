#pragma once
#include "NewEngine.h"
#include "Weapon.h"
#include "Character.h"

struct Weapon;

enum class PartID
{
	Transform,

	Body,	// 菴・
	Head,	// 鬆ｭ

	LeftArm,	// 蟾ｦ閻・
	LeftHand,	// 蟾ｦ謇・
	LeftThigh,	 // 蟾ｦ螟ｪ繧ゅｂ
	LeftLeg,	 // 蟾ｦ雜ｳ

	RightArm,	// 蜿ｳ閻・
	RightHand,	// 蜿ｳ謇・
	RightThigh,	 // 蜿ｳ螟ｪ繧ゅｂ
	RightLeg,	 // 蜿ｳ雜ｳ
};

enum class WeaponPartID
{
	Left, Right
};

class HumanoidBody
{
protected:
	std::array<std::unique_ptr<Object3D>, 11> mParts;
	std::array<Weapon*, 2> mWeapons;
	std::array<Vec3, 13> mCurRots;

public:
	Character* iParent;

	Vec3 pos;
	Vec3 attackPos;
	Vec3 rot;
	Vec3 scale;

public:
	HumanoidBody();
	virtual ~HumanoidBody() {}
	void BaseInit();
	void BaseUpdate();
	void BaseDrawModel();
	void BaseDrawDebugGui();

	void DrawWeapon(const WeaponPartID partID);
public:
	std::vector<Vec3> CalcCurRots();
	std::vector<Vec3> CalcCurWeaponPoses();
	std::vector<Vec3> CalcCurWeaponRots();
	void SetWeapon(Weapon* weapon, const WeaponPartID partID);
	Vec3 GetWorldPos(const PartID partID);
	Object3D* GetPart(const PartID partID);
	Object3D* GetWeaponPart(const WeaponPartID partID);
	uint32_t GetPartsSize();
	uint32_t GetWeaponPartsSize();
};

