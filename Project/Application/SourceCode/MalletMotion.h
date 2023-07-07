#pragma once
#include "IWeaponMotion.h"

// 槌関連のモーション
class MalletMotion : public IWeaponMotion
{
private:
	Vec3 prevPos_;
	Vec3 startPos_;
	float startRotY_;
	float endRotY_;
	float length_;

private:// コンボ 1 ～ 4

	// 手が後ろに引く時
	void Step0MotionInit(HumanoidBody* human);
	void Step0MotionUpdate(HumanoidBody* human);

	// 手が前に倒す時（攻撃するとき）
	void Step1MotionInit(HumanoidBody* human);
	void Step1MotionUpdate(HumanoidBody* human);

	// 待機に戻る時
	void Step2MotionInit(HumanoidBody* human);
	void Step2MotionUpdate(HumanoidBody* human);

	// 各コンボの設定
	void ComboSetting();

	// 現在の回転角を取得
	void CalcCurrentRot(HumanoidBody* human);


public:
	MalletMotion();
	void Init(HumanoidBody* human) override;
	void AttackMotion(HumanoidBody* human) override;
	void HeavyAttackMotion(HumanoidBody* human) override;
};

