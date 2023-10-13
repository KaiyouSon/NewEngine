#pragma once
#include "Weapon.h"

// クラブのクラス
class Club : public Weapon
{
private:
	void ColliderUpdate() override;
	void CalcDamage() override;

public:
	Club();
	void Init() override;
	void Update(Transform* parent = nullptr) override;
	void DrawModel() override;
};
