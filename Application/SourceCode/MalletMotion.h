#pragma once
#include "IWeaponMotion.h"

// 槌関連のモーション
class MalletMotion : public IWeaponMotion
{
private:
	Vec3 curPos_;
	float curRotY_;
	float nextRotY_;

private:
	// 手が後ろに引く時
	void Step0MotionInit(HumanoidBody* human);
	void Step0MotionUpdate(HumanoidBody* human);

	// 手が前に倒す時（攻撃するとき）
	void Step1MotionInit(HumanoidBody* human);
	void Step1MotionUpdate(HumanoidBody* human);

	// 待機に戻る時
	void Step2MotionInit(HumanoidBody* human);
	void Step2MotionUpdate(HumanoidBody* human);

	// 現在の回転角を取得
	void CalcCurrentRot(HumanoidBody* human);

public:
	void Init() override;
	void AttackMotion(HumanoidBody* human) override;

};

