#pragma once
#include "Weapon.h"

class Sword : public Weapon
{
private:
	void ColliderUpdate(bool isCalc) override;

public:
	Sword();
	void Init() override;
	void Update(Transform* parent = nullptr) override;
	void DrawModel() override;
};

