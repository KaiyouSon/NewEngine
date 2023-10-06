#include "MoveMotion.h"
#include "HumanoidBody.h"
#include "Player.h"

MoveMotion::MoveMotion()
{
}

void MoveMotion::Init(PlayerBody* human)
{
	mEase.SetEaseTimer(20);
	mEase.SetPowNum(2);
	mEase.Reset();

	mStep = 0;

	mIsInit = false;
	mIsPlay = false;

	mStartRots.resize(human->GetPartsSize());
	mEndRots.resize(human->GetPartsSize());

	//RotsInit(human);
}

// 繧ｸ繝ｧ繧ｮ繝ｳ繧ｰ繝｢繝ｼ繧ｷ繝ｧ繝ｳ
void MoveMotion::JoggingMotion(PlayerBody* human)
{
	// 譛蛻昴・荳蝗槭＠縺矩壹ｉ縺ｪ縺・・譛溷喧
	if (mIsPlay == false)
	{
		JoggingInit(human);
		mIsPlay = true;
	}

	if (mStep == 0)
	{
		if (mIsInit == false)
		{
			Step0Init();
			mIsInit = true;
		}
		Step0Update(human);
	}
	else if (mStep == 1)
	{
		if (mIsInit == false)
		{
			Step1Init(human);
			mIsInit = true;
		}
		Step1Update(human);
	}
	else if (mStep == 2)
	{
		if (mIsInit == false)
		{
			Step2Init(human);
			mIsInit = true;
		}
		Step2Update(human);
	}
	else if (mStep == 3)
	{
		if (mIsInit == false)
		{
			Step3Init(human);
			mIsInit = true;
		}
		Step3Update(human);
	}
}

// 襍ｰ繧翫Δ繝ｼ繧ｷ繝ｧ繝ｳ
void MoveMotion::RunMotion(PlayerBody* human)
{
	// 譛蛻昴・荳蝗槭＠縺矩壹ｉ縺ｪ縺・・譛溷喧
	if (mIsPlay == false)
	{
		// 蜷・Κ菴阪・隗貞ｺｦ縺ｮ蛻晄悄蛹・
		RunInit(human);
		mIsPlay = true;
	}

	if (mStep == 0)
	{
		if (mIsInit == false)
		{
			Step0Init();
			mIsInit = true;
		}
		Step0Update(human);
	}
	else if (mStep == 1)
	{
		if (mIsInit == false)
		{
			Step1Init(human);
			mIsInit = true;
		}
		Step1Update(human);
	}
	else if (mStep == 2)
	{
		if (mIsInit == false)
		{
			Step2Init(human);
			mIsInit = true;
		}
		Step2Update(human);
	}
	else if (mStep == 3)
	{
		if (mIsInit == false)
		{
			Step3Init(human);
			mIsInit = true;
		}
		Step3Update(human);
	}
}

// 迴ｾ蝨ｨ縺ｮ蟋ｿ蜍｢縺九ｉ襍ｰ繧翫Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ陬憺俣
void MoveMotion::Step0Init()
{
	mEase.SetEaseTimer(10);
	mEase.SetPowNum(2);
	mEase.Reset();
}
void MoveMotion::Step0Update(PlayerBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Body; i < mStartRots.size(); i++)
	{
		human->GetPart((PartID)i)->rot = mEase.InOut(mCurRots[i], mStartRots[i]);
	}

	human->parent->mMoveSpeed = mEase.Lerp(0, mEndSpeed);

	mEase.Update();

	if (mEase.GetisEnd() == true)
	{
		mStep = 1;
		mIsInit = false;
		mEase.Reset();
	}

	if (Pad::GetStick(PadCode::LeftStick, 300) == 0)
	{
		mStep = 2;
		mIsInit = false;
		mEase.Reset();
	}
}

// 襍ｰ繧翫Δ繝ｼ繧ｷ繝ｧ繝ｳ
void MoveMotion::Step1Init(PlayerBody* human)
{
	human = nullptr;

	mEase.SetEaseTimer(20);
	mEase.SetPowNum(2);
}
void MoveMotion::Step1Update(PlayerBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Body; i < mStartRots.size(); i++)
	{
		human->GetPart((PartID)i)->rot = mEase.InOut(mStartRots[i], mEndRots[i]);
	}

	mEase.Update();

	if (mEase.GetisEnd() == true)
	{
		mEase.Reset();

		ReverceRots();
		mCount = (mCount == 0) ? 1 : 0;
	}

	if (Pad::GetStick(PadCode::LeftStick, 300) == 0)
	{
		mStep = 2;
		mIsInit = false;
		mEase.Reset();
	}
}

// 蠕・ｩ溷ｧｿ蜍｢縺ｫ謌ｻ繧・
void MoveMotion::Step2Init(PlayerBody* human)
{
	mEase.SetEaseTimer(10);
	mEase.SetPowNum(2);

	mCurSpeed = human->parent->mMoveSpeed;

	CalcCurrentRot(human);
}
void MoveMotion::Step2Update(PlayerBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Body; i < mStartRots.size(); i++)
	{
		human->GetPart((PartID)i)->rot = mEase.InOut(mCurRots[i], 0);
	}
	human->parent->mMoveSpeed = mEase.Lerp(mCurSpeed, 0);
	mEase.Update();

	if (mEase.GetisEnd() == true)
	{
		mIsInit = false;
		mIsPlay = false;
		mEase.Reset();
	}
}

// 繝繝ｼ繝悶Δ繝ｼ繧ｷ繝ｧ繝ｳ縺悟､峨ｏ縺｣縺滓凾縺ｫ陬憺俣縺吶ｋ
void MoveMotion::Step3Init(PlayerBody* human)
{
	mEase.SetEaseTimer(10);
	mEase.SetPowNum(2);
	mEase.Reset();

	mCurSpeed = human->parent->mMoveSpeed;

	if (mCount == 1)
	{
		ReverceRots();
	}
}
void MoveMotion::Step3Update(PlayerBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Body; i < mEndRots.size(); i++)
	{
		human->GetPart((PartID)i)->rot = mEase.Lerp(mCurRots[i], mEndRots[i]);
	}

	human->parent->mMoveSpeed = mEase.Lerp(mCurSpeed, mEndSpeed);

	mEase.Update();

	if (mEase.GetisEnd() == true)
	{
		mStep = 1;

		mEase.Reset();

		mIsInit = false;
		mCount = (mCount == 0) ? 1 : 0;
		ReverceRots();
	}
}

// 繧ｸ繝ｧ繧ｮ繝ｳ繧ｰ縺ｮ蛻晄悄蛹・
void MoveMotion::JoggingInit(PlayerBody* human)
{
	mIsInit = false;

	mStep = mIsPlay ? 3 : 0;

	// 迴ｾ蝨ｨ縺ｮ隗貞ｺｦ繧定ｨ育ｮ・
	mCurRots = human->CalcCurRots();
	// 邨ゆｺ・溷ｺｦ縺ｮ蜿門ｾ・
	mEndSpeed = human->parent->mJoggingSpeed;

	// 蜷・Κ菴阪・隗貞ｺｦ縺ｮ蛻晄悄蛹・
	mStartRots[(uint32_t)PartID::Head] = Radian(Vec3(0, -2, 0));
	mStartRots[(uint32_t)PartID::Body] = Radian(Vec3(5, 10, 0));
	mStartRots[(uint32_t)PartID::RightArm] = Radian(Vec3(45, 0, 5));
	mStartRots[(uint32_t)PartID::RightHand] = Radian(Vec3(-70, 0, 0));
	mStartRots[(uint32_t)PartID::LeftArm] = Radian(Vec3(-25, 0, -5));
	mStartRots[(uint32_t)PartID::LeftHand] = Radian(Vec3(-50, 0, 0));
	mStartRots[(uint32_t)PartID::RightThigh] = Radian(Vec3(-60, 0, 0));
	mStartRots[(uint32_t)PartID::RightLeg] = Radian(Vec3(30, 0, 0));
	mStartRots[(uint32_t)PartID::LeftThigh] = Radian(Vec3(40, 0, 0));
	mStartRots[(uint32_t)PartID::LeftLeg] = Radian(Vec3(30, 0, 0));

	mEndRots[(uint32_t)PartID::Head] = Radian(Vec3(0, 2, 0));
	mEndRots[(uint32_t)PartID::Body] = Radian(Vec3(5, -10, 0));
	mEndRots[(uint32_t)PartID::RightArm] = Radian(Vec3(-25, 0, 5));
	mEndRots[(uint32_t)PartID::RightHand] = Radian(Vec3(-50, 0, 0));
	mEndRots[(uint32_t)PartID::LeftArm] = Radian(Vec3(45, 0, -5));
	mEndRots[(uint32_t)PartID::LeftHand] = Radian(Vec3(-70, 0, 0));
	mEndRots[(uint32_t)PartID::RightThigh] = Radian(Vec3(40, 0, 0));
	mEndRots[(uint32_t)PartID::RightLeg] = Radian(Vec3(10, 0, 0));
	mEndRots[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-60, 0, 0));
	mEndRots[(uint32_t)PartID::LeftLeg] = Radian(Vec3(10, 0, 0));
}

// 襍ｰ繧翫・蛻晄悄蛹・
void MoveMotion::RunInit(PlayerBody* human)
{
	mIsInit = false;

	mStep = mIsPlay ? 3 : 0;

	// 迴ｾ蝨ｨ縺ｮ隗貞ｺｦ繧定ｨ育ｮ・
	mCurRots = human->CalcCurRots();
	// 邨ゆｺ・溷ｺｦ縺ｮ蜿門ｾ・
	mEndSpeed = human->parent->mRunSpeed;

	// 蜷・Κ菴阪・隗貞ｺｦ縺ｮ蛻晄悄蛹・
	mStartRots[(uint32_t)PartID::Head] = Radian(Vec3(0, -10, 0));
	mStartRots[(uint32_t)PartID::Body] = Radian(Vec3(15, 10, 0));
	mStartRots[(uint32_t)PartID::RightArm] = Radian(Vec3(60, 0, 10));
	mStartRots[(uint32_t)PartID::RightHand] = Radian(Vec3(-80, 0, 0));
	mStartRots[(uint32_t)PartID::LeftArm] = Radian(Vec3(-40, 0, -10));
	mStartRots[(uint32_t)PartID::LeftHand] = Radian(Vec3(-50, 0, 0));
	mStartRots[(uint32_t)PartID::RightThigh] = Radian(Vec3(-80, 0, 0));
	mStartRots[(uint32_t)PartID::RightLeg] = Radian(Vec3(15, 0, 0));
	mStartRots[(uint32_t)PartID::LeftThigh] = Radian(Vec3(50, 0, 0));
	mStartRots[(uint32_t)PartID::LeftLeg] = Radian(Vec3(30, 0, 0));

	mEndRots[(uint32_t)PartID::Head] = Radian(Vec3(0, 10, 0));
	mEndRots[(uint32_t)PartID::Body] = Radian(Vec3(15, -10, 0));
	mEndRots[(uint32_t)PartID::RightArm] = Radian(Vec3(-40, 0, 10));
	mEndRots[(uint32_t)PartID::RightHand] = Radian(Vec3(-50, 0, 0));
	mEndRots[(uint32_t)PartID::LeftArm] = Radian(Vec3(60, 0, -10));
	mEndRots[(uint32_t)PartID::LeftHand] = Radian(Vec3(-80, 0, 0));
	mEndRots[(uint32_t)PartID::RightThigh] = Radian(Vec3(50, 0, 0));
	mEndRots[(uint32_t)PartID::RightLeg] = Radian(Vec3(30, 0, 0));
	mEndRots[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-80, 0, 0));
	mEndRots[(uint32_t)PartID::LeftLeg] = Radian(Vec3(15, 0, 0));
}

// 迴ｾ蝨ｨ縺ｮ隗貞ｺｦ繧定ｨ育ｮ励☆繧・
void MoveMotion::CalcCurrentRot(PlayerBody* human)
{
	mCurRots.resize(human->GetPartsSize());

	for (uint32_t i = 0; i < human->GetPartsSize(); i++)
	{
		mCurRots[i] = human->GetPart((PartID)i)->rot;
	}
}

// 髢句ｧ句屓霆｢隗貞ｺｦ縺ｨ邨ゆｺ・屓霆｢隗貞ｺｦ繧貞渚霆｢縺吶ｋ
void MoveMotion::ReverceRots()
{
	std::vector<Vec3> temp;
	temp.resize(mStartRots.size());

	for (uint32_t i = 0; i < mStartRots.size(); i++)
	{
		// 繧ｹ繧ｿ繝ｼ繝医ｒ譬ｼ邏阪☆繧・
		temp[i] = mStartRots[i];

		// 繧ｨ繝ｳ繝峨ｒ繧ｹ繧ｿ繝ｼ繝医↓荳頑嶌縺阪☆繧・
		mStartRots[i] = mEndRots[i];

		//譬ｼ邏阪＠縺溘せ繧ｿ繝ｼ繝医ｒ繧ｨ繝ｳ繝峨↓荳頑嶌縺阪☆繧・
		mEndRots[i] = temp[i];
	}
}

bool MoveMotion::GetisPlay()
{
	return mIsPlay;
}

