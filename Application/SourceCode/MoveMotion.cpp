#include "MoveMotion.h"
#include "HumanoidBody.h"
#include "Player.h"

MoveMotion::MoveMotion()
{
}

void MoveMotion::Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(20);
	ease_.SetPowNum(2);
	ease_.Reset();

	step_ = 0;

	isInit_ = false;
	isPlay_ = false;

	startRots_.resize(human->GetPartsSize());
	endRots_.resize(human->GetPartsSize());

	//RotsInit(human);
}

// ジョギングモーション
void MoveMotion::JoggingMotion(HumanoidBody* human)
{
	// 最初の一回しか通らない初期化
	if (isPlay_ == false)
	{
		JoggingInit(human);
		isPlay_ = true;
	}

	if (step_ == 0)
	{
		if (isInit_ == false)
		{
			Step0Init(human);
			isInit_ = true;
		}
		Step0Update(human);
	}
	else if (step_ == 1)
	{
		if (isInit_ == false)
		{
			Step1Init(human);
			isInit_ = true;
		}
		Step1Update(human);
	}
	else if (step_ == 2)
	{
		if (isInit_ == false)
		{
			Step2Init(human);
			isInit_ = true;
		}
		Step2Update(human);
	}
	else if (step_ == 3)
	{
		if (isInit_ == false)
		{
			Step3Init(human);
			isInit_ = true;
		}
		Step3Update(human);
	}
}

// 走りモーション
void MoveMotion::RunMotion(HumanoidBody* human)
{
	// 最初の一回しか通らない初期化
	if (isPlay_ == false)
	{
		// 各部位の角度の初期化
		RunInit(human);

		isPlay_ = true;
	}

	if (step_ == 0)
	{
		if (isInit_ == false)
		{
			Step0Init(human);
			isInit_ = true;
		}
		Step0Update(human);
	}
	else if (step_ == 1)
	{
		if (isInit_ == false)
		{
			Step1Init(human);
			isInit_ = true;
		}
		Step1Update(human);
	}
	else if (step_ == 2)
	{
		if (isInit_ == false)
		{
			Step2Init(human);
			isInit_ = true;
		}
		Step2Update(human);
	}
	else if (step_ == 3)
	{
		if (isInit_ == false)
		{
			Step3Init(human);
			isInit_ = true;
		}
		Step3Update(human);
	}
}

// 現在の姿勢から走りモーションに補間
void MoveMotion::Step0Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(10);
	ease_.SetPowNum(2);
	ease_.Reset();
}
void MoveMotion::Step0Update(HumanoidBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Head; i < startRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.InOut(curRots_[i], startRots_[i]);
	}

	human->parent->moveSpeed_ = ease_.Lerp(0, endSpeed_);

	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 1;
		isInit_ = false;
		ease_.Reset();
	}

	if (Pad::GetStick(PadCode::LeftStick, 300) == 0)
	{
		step_ = 2;
		isInit_ = false;
		ease_.Reset();
	}
}

// 走りモーション
void MoveMotion::Step1Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(20);
	ease_.SetPowNum(2);
}
void MoveMotion::Step1Update(HumanoidBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Head; i < startRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.InOut(startRots_[i], endRots_[i]);
	}

	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		ease_.Reset();

		ReverceRots();
		count_ = (count_ == 0) ? 1 : 0;
	}

	if (Pad::GetStick(PadCode::LeftStick, 300) == 0)
	{
		step_ = 2;
		isInit_ = false;
		ease_.Reset();
	}
}

// 待機姿勢に戻る
void MoveMotion::Step2Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(10);
	ease_.SetPowNum(2);

	curSpeed_ = human->parent->moveSpeed_;

	CalcCurrentRot(human);
}
void MoveMotion::Step2Update(HumanoidBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Head; i < startRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.InOut(curRots_[i], 0);
	}
	human->parent->moveSpeed_ = ease_.Lerp(curSpeed_, 0);
	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		isInit_ = false;
		isPlay_ = false;
		ease_.Reset();
	}
}

// ムーブモーションが変わった時に補間する
void MoveMotion::Step3Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(10);
	ease_.SetPowNum(2);
	ease_.Reset();

	curSpeed_ = human->parent->moveSpeed_;

	if (count_ == 1)
	{
		ReverceRots();
	}
}
void MoveMotion::Step3Update(HumanoidBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Head; i < endRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.InOut(curRots_[i], endRots_[i]);
	}

	human->parent->moveSpeed_ = ease_.Lerp(curSpeed_, endSpeed_);

	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 1;

		ease_.Reset();

		isInit_ = false;
		count_ = (count_ == 0) ? 1 : 0;
		ReverceRots();
	}
}

// ジョギングの初期化
void MoveMotion::JoggingInit(HumanoidBody* human)
{
	isInit_ = false;

	step_ = isPlay_ ? 3 : 0;

	// 現在の角度を計算
	curRots_ = human->CalcCurRots();
	// 終了速度の取得
	endSpeed_ = human->parent->joggingSpeed_;

	// 各部位の角度の初期化
	startRots_[(uint32_t)PartID::Head] = Radian(Vec3(0, -2, 0));
	startRots_[(uint32_t)PartID::Body] = Radian(Vec3(5, 10, 0));
	startRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(35, 0, 5));
	startRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-70, 0, 0));
	startRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(-15, 0, -5));
	startRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-50, 0, 0));
	startRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(-50, 0, 0));
	startRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(30, 0, 0));
	startRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(30, 0, 0));
	startRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(30, 0, 0));

	endRots_[(uint32_t)PartID::Head] = Radian(Vec3(0, 2, 0));
	endRots_[(uint32_t)PartID::Body] = Radian(Vec3(5, -10, 0));
	endRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(-15, 0, 5));
	endRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-50, 0, 0));
	endRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(35, 0, -5));
	endRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-70, 0, 0));
	endRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(30, 0, 0));
	endRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(10, 0, 0));
	endRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-50, 0, 0));
	endRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(10, 0, 0));
}

// 走りの初期化
void MoveMotion::RunInit(HumanoidBody* human)
{
	isInit_ = false;

	step_ = isPlay_ ? 3 : 0;

	// 現在の角度を計算
	curRots_ = human->CalcCurRots();
	// 終了速度の取得
	endSpeed_ = human->parent->runSpeed_;

	// 各部位の角度の初期化
	startRots_[(uint32_t)PartID::Head] = Radian(Vec3(0, -10, 0));
	startRots_[(uint32_t)PartID::Body] = Radian(Vec3(15, 10, 0));
	startRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(60, 0, 10));
	startRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-80, 0, 0));
	startRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(-40, 0, -10));
	startRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-50, 0, 0));
	startRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(-80, 0, 0));
	startRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(15, 0, 0));
	startRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(50, 0, 0));
	startRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(30, 0, 0));

	endRots_[(uint32_t)PartID::Head] = Radian(Vec3(0, 10, 0));
	endRots_[(uint32_t)PartID::Body] = Radian(Vec3(15, -10, 0));
	endRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(-40, 0, 10));
	endRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-50, 0, 0));
	endRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(60, 0, -10));
	endRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-80, 0, 0));
	endRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(50, 0, 0));
	endRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(30, 0, 0));
	endRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-80, 0, 0));
	endRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(15, 0, 0));
}

// 現在の角度を計算する
void MoveMotion::CalcCurrentRot(HumanoidBody* human)
{
	curRots_.resize(human->GetPartsSize());

	for (uint32_t i = 0; i < human->GetPartsSize(); i++)
	{
		curRots_[i] = human->GetPart((PartID)i)->rot;
	}
}

// 開始回転角度と終了回転角度を反転する
void MoveMotion::ReverceRots()
{
	std::vector<Vec3> temp;
	temp.resize(startRots_.size());

	for (uint32_t i = 0; i < startRots_.size(); i++)
	{
		// スタートを格納する
		temp[i] = startRots_[i];

		// エンドをスタートに上書きする
		startRots_[i] = endRots_[i];

		//格納したスタートをエンドに上書きする
		endRots_[i] = temp[i];
	}
}

bool MoveMotion::GetisPlay()
{
	return isPlay_;
}
