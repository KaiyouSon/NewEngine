#include "MalletMotion.h"
#include "PlayerBody.h"
#include "CollisionManager.h"

MalletMotion::MalletMotion()
{
	mMotions.emplace_back(MotionManager::GetMotion("MalletWeakAttack1"));
	mMotions.emplace_back(MotionManager::GetMotion("MalletWeakAttack2"));
	mMotions.emplace_back(MotionManager::GetMotion("MalletWeakAttack3"));
	mMotions.emplace_back(MotionManager::GetMotion("MalletHeavyAttack"));
	mMotions.emplace_back(MotionManager::GetMotion("MalletBackAttack"));
	mMotions.emplace_back(MotionManager::GetMotion("MalletRollAttack"));
}

void MalletMotion::Init(HumanoidBody* human)
{
	mIsPlay = false;
	mIsInit = false;
	mIsCanChangeMotion = false;
	mIsCalcCollider = false;
	mStep = 0;
	mEase.Reset();
	mCurRots.clear();
	mComboMaxCount = 5;

	mCurRots.resize(human->GetPartsSize());
	mEndRots.resize(human->GetPartsSize());
	mCurWeaponRots.resize(human->GetWeaponPartsSize());
	mEndWeaponRots.resize(human->GetWeaponPartsSize());

	// 蜀咲函邨ゅｏ縺｣縺滓凾縺ｮ蛻晄悄蛹・
	if (mComboCount > mComboMaxCount)
	{
		// 繧ｳ繝ｳ繝應ｸｭ縺ｮ蛻晄悄蛹悶＠縺ｪ縺・◆繧・
		ResetComboCount();
	}

	human->GetPart(PartID::Body)->pos.y = 0.f;

}
void MalletMotion::WeakMotion(PlayerBody* human)
{
	if (mIsPlay == false)
	{
		mIsPlay = true;
		mAttackType = AttackType::Weak;
	}

	mEase.Update();

	if (mIsInit == false)
	{
		CurrentStepInit(human);
		if (mComboCount == 1)
		{
			BaseInit(human, 0);
		}
		else if (mComboCount < mComboMaxCount)
		{
			BaseInit(human, 1);
		}
		else
		{
			BaseInit(human, 2);
		}
		mIsInit = true;
	}

	if (mComboCount == 1)
	{
		BasePrevUpdate(human);
	}
	else if (mComboCount < mComboMaxCount)
	{
		BasePrevUpdate(human);
	}
	else
	{
		BasePrevUpdate(human);
	}

	CurrentStepUpdate(human);

	if (mComboCount == 1)
	{
		BasePostUpdate(0);
	}
	else if (mComboCount < mComboMaxCount)
	{
		BasePostUpdate(1);
	}
	else
	{
		BasePostUpdate(2);
	}
}
void MalletMotion::HeavyMotion(PlayerBody* human)
{
	if (mIsPlay == false)
	{
		mIsPlay = true;
		mAttackType = AttackType::Heavy;
	}
	mEase.Update();

	if (mIsInit == false)
	{
		CurrentStepInit(human);
		BaseInit(human, 3);
		mIsInit = true;
	}
	BasePrevUpdate(human);
	CurrentStepUpdate(human);
	BasePostUpdate(3);
}
void MalletMotion::BackMotion(PlayerBody* human)
{
	if (mIsPlay == false)
	{
		mIsPlay = true;
		mAttackType = AttackType::Back;
	}
	mEase.Update();

	if (mIsInit == false)
	{
		CurrentStepInit(human);
		BaseInit(human, 4);
		mIsInit = true;
	}
	BasePrevUpdate(human);
	CurrentStepUpdate(human);
	BasePostUpdate(4);
}
void MalletMotion::RollMotion(PlayerBody* human)
{
	if (mIsPlay == false)
	{
		mIsPlay = true;
		mAttackType = AttackType::Roll;
	}
	mEase.Update();

	if (mIsInit == false)
	{
		CurrentStepInit(human);
		BaseInit(human, 5);

		mIsInit = true;
	}
	BasePrevUpdate(human);
	CurrentStepUpdate(human);
	BasePostUpdate(5);
}

void MalletMotion::CurrentStepInit(PlayerBody* human)
{
	if (mAttackType == AttackType::Weak)
	{
		if (mStep == 1)
		{
			WeakStep1Init(human);
		}
	}
	else if (mAttackType == AttackType::Heavy)
	{
		if (mStep == 0)
		{
			HeavyStep0Init(human);
		}
		else if (mStep == 1)
		{
			HeavyStep1Init();
		}
		else if (mStep == 2)
		{
			HeavyStep2Init(human);
		}
		else if (mStep == 3)
		{
			HeavyStep3Init();
		}
		else if (mStep == 4)
		{
			HeavyStep4Init();
		}
		else if (mStep == 5)
		{
			HeavyStep5Init();
		}
		else if (mStep == 6)
		{
			HeavyStep6Init();
		}
	}
	else if (mAttackType == AttackType::Back)
	{
		if (mStep == 0)
		{
			BackStep0Init(human);
		}
		else if (mStep == 2)
		{
			BackStep2Init(human);
		}
	}
	else if (mAttackType == AttackType::Roll)
	{
		if (mStep == 0)
		{
			RollStep0Init(human);
		}
	}
}
void MalletMotion::CurrentStepUpdate(PlayerBody* human)
{
	if (mAttackType == AttackType::Weak)
	{
		if (mStep == 1)
		{
			WeakStep1Update(human);
		}
	}
	else if (mAttackType == AttackType::Heavy)
	{
		if (mStep == 0)
		{
			HeavyStep0Update(human);
		}
		else if (mStep == 1)
		{
			HeavyStep1Update(human);
		}
		else if (mStep == 2)
		{
			HeavyStep2Update(human);
		}
		else if (mStep == 3)
		{
			HeavyStep3Update(human);
		}
		else if (mStep == 3)
		{
			HeavyStep4Update(human);
		}
		else if (mStep == 5)
		{
			HeavyStep5Update(human);
		}
		else if (mStep == 6)
		{
			HeavyStep6Update(human);
		}
	}
	else if (mAttackType == AttackType::Back)
	{
		if (mStep == 0)
		{
			BackStep0Update(human);
		}
		else if (mStep == 2)
		{
			BackStep2Update(human);
		}
		else if (mStep == 3)
		{
			BackStep3Update(human);
		}
	}
	else if (mAttackType == AttackType::Roll)
	{
		if (mStep == 0)
		{
			RollStep0Update(human);
		}
		else if (mStep == 1)
		{
			RollStep1Update(human);
		}
	}
}

// 蠑ｱ謾ｻ謦・
void MalletMotion::WeakStep1Init(PlayerBody* human)
{
	// 蜑阪・繧ｯ繝医Ν縺ｮ險育ｮ・
	human->parent->CalcFrontVec();

	// 謾ｻ謦・Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｧ騾ｲ繧霍晞屬縺ｮ險育ｮ・
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(human->parent->mFrontVec, 8);

	// 迴ｾ蝨ｨ縺ｮ蠎ｧ讓吶ｒ蜿門ｾ・
	startPos_ = human->pos;

	// 蜈･蜉帙＠縺溷ｾ後・蝗櫁ｻ｢隗偵ｒ蜿門ｾ・
	startRotY_ = human->rot.y;
	endRotY_ = atan2f(human->parent->mFrontVec.x, human->parent->mFrontVec.z);

	// 蠖薙◆繧雁愛螳壽怏蜉ｹ
	mIsCalcCollider = true;
}
void MalletMotion::WeakStep1Update(PlayerBody* human)
{
	// 蟆代＠蜑阪↓遘ｻ蜍輔☆繧句・逅・
	const Vec3 endPos = startPos_ + human->parent->mFrontVec.Norm() * length_;
	human->pos = mEase.InOut(startPos_, endPos);
	human->rot.y = mEase.InOut(startRotY_, endRotY_);
	human->parent->mMoveVel = endPos - startPos_;

	if (mEase.GetTimer() == 15)
	{
		SoundManager::Play("WeakAttackSE");
	}

	if (mEase.GetisEnd() == true)
	{
		if (mComboCount < mComboMaxCount)
		{
			// 繧ｳ繝ｳ繝懊〒縺阪ｋ繝輔Λ繝・
			mIsCanChangeMotion = true;
		}

		// 蠖薙◆繧雁愛螳夂┌蜉ｹ
		mIsCalcCollider = false;
	}
}

// 蠑ｷ謾ｻ謦・
void MalletMotion::HeavyStep0Init(PlayerBody* human)
{
	startPosY = human->GetPart(PartID::Body)->pos.y;
	endPosY = -0.62f;
}
void MalletMotion::HeavyStep0Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = mEase.In(startPosY, endPosY);
}
void MalletMotion::HeavyStep1Init()
{
	startPosY = endPosY;
	endPosY = -1.04f;
}
void MalletMotion::HeavyStep1Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = mEase.In(startPosY, endPosY);

	if (!Pad::GetTrigger(PadCode::RightTrigger, 300))
	{
		mEase.SetisEnd(true);
	}

	if (mEase.GetisEnd() == true)
	{
		Player* player = static_cast<Player*>(human->iParent);
		player->mWeapon->SetChargeRate(1.f + mEase.GetTimeRate());
	}
}
void MalletMotion::HeavyStep2Init(PlayerBody* human)
{
	// 蜑阪・繧ｯ繝医Ν縺ｮ險育ｮ・
	human->parent->CalcFrontVec();

	// 謾ｻ謦・Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｧ騾ｲ繧霍晞屬縺ｮ險育ｮ・
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(human->parent->mFrontVec, 10);

	// 迴ｾ蝨ｨ縺ｮ蠎ｧ讓吶ｒ蜿門ｾ・
	startPos_ = human->pos;

	// 蜈･蜉帙＠縺溷ｾ後・蝗櫁ｻ｢隗偵ｒ蜿門ｾ・
	startRotY_ = human->rot.y;
	endRotY_ = atan2f(human->parent->mFrontVec.x, human->parent->mFrontVec.z);

	startPosY = endPosY;
	endPosY = -1.3f;
}
void MalletMotion::HeavyStep2Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = mEase.In(startPosY, endPosY);

	// 蟆代＠蜑阪↓遘ｻ蜍輔☆繧句・逅・
	const Vec3 endPos = startPos_ + human->parent->mFrontVec.Norm() * length_;
	human->pos = mEase.InOut(startPos_, endPos);
	human->rot.y = mEase.InOut(startRotY_, endRotY_);
	human->parent->mMoveVel = endPos - startPos_;

	if (mEase.GetisEnd() == true)
	{
		SoundManager::Play("WeakAttackSE");
		// 蠖薙◆繧雁愛螳壽怏蜉ｹ
		mIsCalcCollider = true;
		//if (mComboCount < mComboMaxCount)
		//{
		//	// 繧ｳ繝ｳ繝懊〒縺阪ｋ繝輔Λ繝・
		//	mIsCanChangeMotion = true;
		//}
	}
}
void MalletMotion::HeavyStep3Init()
{
	startPosY = endPosY;
	endPosY = -1.85f;
}
void MalletMotion::HeavyStep3Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = mEase.In(startPosY, endPosY);

	if (mEase.GetisEnd() == true)
	{
		// 蠖薙◆繧雁愛螳夂┌蜉ｹ
		mIsCalcCollider = false;
	}
}
void MalletMotion::HeavyStep4Init()
{
	startPosY = endPosY;
	endPosY = -1.2f;
}
void MalletMotion::HeavyStep4Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = mEase.In(startPosY, endPosY);
}
void MalletMotion::HeavyStep5Init()
{
	startPosY = endPosY;
	endPosY = -0.32f;
}
void MalletMotion::HeavyStep5Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = mEase.In(startPosY, endPosY);

	if (mEase.GetisEnd() == true)
	{
		mIsCanChangeMotion = true;
	}
}
void MalletMotion::HeavyStep6Init()
{
	startPosY = endPosY;
	endPosY = 0.f;
}
void MalletMotion::HeavyStep6Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = mEase.In(startPosY, endPosY);
}

// 繝舌ャ繧ｯ謾ｻ謦・
void MalletMotion::BackStep0Init(PlayerBody* human)
{
	// 蜑阪・繧ｯ繝医Ν縺ｮ險育ｮ・
	human->parent->CalcFrontVec();

	// 謾ｻ謦・Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｧ騾ｲ繧霍晞屬縺ｮ險育ｮ・
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(-human->parent->mFrontVec, 2);

	// 迴ｾ蝨ｨ縺ｮ蠎ｧ讓吶ｒ蜿門ｾ・
	startPos_ = human->pos;

	// 蜈･蜉帙＠縺溷ｾ後・蝗櫁ｻ｢隗偵ｒ蜿門ｾ・
	startRotY_ = human->rot.y;
	endRotY_ = atan2f(human->parent->mFrontVec.x, human->parent->mFrontVec.z);

	moveEase_.SetEaseTimer(5);
	moveEase_.SetPowNum(2);
	moveEase_.Reset();
}
void MalletMotion::BackStep0Update(PlayerBody* human)
{
	moveEase_.Update();

	// 蟆代＠蜑阪↓遘ｻ蜍輔☆繧句・逅・
	const Vec3 endPos = startPos_ - human->parent->mFrontVec.Norm() * length_;
	human->pos = moveEase_.InOut(startPos_, endPos);
	human->parent->mMoveVel = endPos - startPos_;
}
void MalletMotion::BackStep2Init(PlayerBody* human)
{
	// 蜑阪・繧ｯ繝医Ν縺ｮ險育ｮ・
	human->parent->CalcFrontVec();

	// 謾ｻ謦・Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｧ騾ｲ繧霍晞屬縺ｮ險育ｮ・
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(human->parent->mFrontVec, 17.5);

	// 迴ｾ蝨ｨ縺ｮ蠎ｧ讓吶ｒ蜿門ｾ・
	startPos_ = human->pos;

	// 蜈･蜉帙＠縺溷ｾ後・蝗櫁ｻ｢隗偵ｒ蜿門ｾ・
	startRotY_ = human->rot.y;
	endRotY_ = atan2f(human->parent->mFrontVec.x, human->parent->mFrontVec.z);

	// 蠖薙◆繧雁愛螳壽怏蜉ｹ
	mIsCalcCollider = true;

	moveEase_.SetEaseTimer(15);
	moveEase_.SetPowNum(2);
	moveEase_.Reset();
}
void MalletMotion::BackStep2Update(PlayerBody* human)
{
	moveEase_.Update();

	// 蟆代＠蜑阪↓遘ｻ蜍輔☆繧句・逅・
	const Vec3 endPos = startPos_ + human->parent->mFrontVec.Norm() * length_;
	human->pos = moveEase_.InOut(startPos_, endPos);
	human->parent->mMoveVel = endPos - startPos_;

	if (mEase.GetisEnd() == true)
	{
		SoundManager::Play("WeakAttackSE");
	}
}
void MalletMotion::BackStep3Update(PlayerBody* human)
{
	moveEase_.Update();

	// 蟆代＠蜑阪↓遘ｻ蜍輔☆繧句・逅・
	const Vec3 endPos = startPos_ + human->parent->mFrontVec.Norm() * length_;
	human->pos = moveEase_.InOut(startPos_, endPos);
	human->parent->mMoveVel = endPos - startPos_;

	if (mEase.GetisEnd() == true)
	{
		// 蠖薙◆繧雁愛螳夂┌蜉ｹ
		mIsCalcCollider = false;
	}

}

// 蝗櫁ｻ｢謾ｻ謦・
void MalletMotion::RollStep0Init(PlayerBody* human)
{
	// 蜑阪・繧ｯ繝医Ν縺ｮ險育ｮ・
	human->parent->CalcFrontVec();

	// 謾ｻ謦・Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｧ騾ｲ繧霍晞屬縺ｮ險育ｮ・
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(-human->parent->mFrontVec, 8);

	// 迴ｾ蝨ｨ縺ｮ蠎ｧ讓吶ｒ蜿門ｾ・
	startPos_ = human->pos;

	// 蜈･蜉帙＠縺溷ｾ後・蝗櫁ｻ｢隗偵ｒ蜿門ｾ・
	startRotY_ = human->rot.y;
	endRotY_ = atan2f(human->parent->mFrontVec.x, human->parent->mFrontVec.z);

	// 蠖薙◆繧雁愛螳壽怏蜉ｹ
	mIsCalcCollider = true;

	moveEase_.SetEaseTimer(30);
	moveEase_.SetPowNum(2);
	moveEase_.Reset();
}
void MalletMotion::RollStep0Update(PlayerBody* human)
{
	moveEase_.Update();

	// 蟆代＠蜑阪↓遘ｻ蜍輔☆繧句・逅・
	const Vec3 endPos = startPos_ + human->parent->mFrontVec.Norm() * length_;
	human->pos = moveEase_.InOut(startPos_, endPos);
	human->parent->mMoveVel = endPos - startPos_;

	if (mEase.GetisEnd() == true)
	{
		SoundManager::Play("WeakAttackSE");
	}
}
void MalletMotion::RollStep1Update(PlayerBody* human)
{
	moveEase_.Update();

	// 蟆代＠蜑阪↓遘ｻ蜍輔☆繧句・逅・
	const Vec3 endPos = startPos_ + human->parent->mFrontVec.Norm() * length_;
	human->pos = moveEase_.InOut(startPos_, endPos);
	human->parent->mMoveVel = endPos - startPos_;

	if (mEase.GetisEnd() == true)
	{
		// 蠖薙◆繧雁愛螳夂┌蜉ｹ
		mIsCalcCollider = false;
	}
}
