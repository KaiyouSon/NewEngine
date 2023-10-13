#pragma once
#include "Weapon.h"

// 剣のクラス
class Sword : public Weapon
{
private:
	void ColliderUpdate() override;
	void CalcDamage() override;

public:
	Sword();
	void Init() override;
	void Update(Transform* parent = nullptr) override;
	void DrawModel() override;
};

