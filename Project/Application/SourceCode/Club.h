#pragma once
#include "Weapon.h"

struct Club : public Weapon
{
private:
	void ColliderUpdate(bool isCalc) override;

public:
	Club();
	void Init() override;
	void Update(Transform* parent = nullptr) override;
	void DrawModel() override;
};

