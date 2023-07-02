#pragma once
#include "Weapon.h"

struct Club : public Weapon
{
public:
	Club();
	void Init() override;
	void Update(Transform* parent = nullptr) override;
	void DrawModel() override;
};

