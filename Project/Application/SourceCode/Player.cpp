#include "Player.h"
#include "GaugeType.h"
#include "Club.h"
#include "Sword.h"

Player::Player() :
	mPlayer(std::make_unique<PlayerBody>()),
	weapon_(std::make_unique<Club>()),
	frontVec_(Vec3::front)
{
}

void Player::Init()
{
	GaugeParamInit();
	mPlayer->Init();
	mPlayer->pos.y = 4.75f;

	mPlayer->SetWeapon(weapon_.get(), WeaponPartID::Right);
	mPlayer->iParent = this;
	mPlayer->parent = static_cast<Player*>(mPlayer->iParent);

	joggingSpeed_ = 0.7f;
	runSpeed_ = 1.2f;

	pushTimer.SetLimitTimer(20);
	damageCoolTimer_.SetLimitTimer(30);

	isAlive_ = true;
	isDissolve_ = false;

	weapon_->weapon->isUseDissolve = true;
	weapon_->weapon->colorPower = 5;
	weapon_->weapon->dissolveColor = Color(255, 30, 0, 255);

	bottleNum_ = 4;
}
void Player::PrevUpdate()
{
	moveVel = 0;

	if (isAlive_ == true)
	{
		// 関数ポインタ
		void (Player:: * pFunc[])() =
		{
			// 登録
			&Player::IdleUpdate,
			&Player::JoggingUpdate,
			&Player::RunUpdate,
			&Player::BackstepUpdate,
			&Player::RollUpdate,
			&Player::AttackR1Update,
			&Player::AttackR2Update,
			&Player::AttackBackUpdate,
			&Player::AttackRollUpdate,
			&Player::DrinkUpdate,
		};

		// 実行
		(this->*pFunc[(int)state_])();
	}

	GaugeParamUpdate();
	ColliderUpdate();
	DamageUpdate();

	if (isAlive_ == false)
	{
		for (uint32_t i = 1; i < mPlayer->parts_.size(); i++)
		{
			mPlayer->parts_[i]->dissolve += 0.005f;
			mPlayer->parts_[i]->dissolve = Min(mPlayer->parts_[i]->dissolve, 2.f);
		}

		weapon_->weapon->dissolve += 0.005f;
		weapon_->weapon->dissolve = Min(weapon_->weapon->dissolve, 2.f);

		if (mPlayer->parts_[1]->dissolve >= 0.5f)
		{
			if (isDissolve_ == false)
			{
				isDissolve_ = true;
			}
		}
	}
}
void Player::PostUpdate()
{
	ProcessAtDebugBulid([&]()
		{
			static bool flag = false;
			if (Key::GetKeyDown(DIK_SPACE))
			{
				flag = true;
				for (uint32_t i = 0; i < mPlayer->parts_.size(); i++)
				{
					mPlayer->parts_[i]->dissolve = 0.f;
				}

			}
			if (flag == true)
			{
				for (uint32_t i = 0; i < mPlayer->parts_.size(); i++)
				{
					mPlayer->parts_[i]->dissolve += 0.005f;
					mPlayer->parts_[i]->dissolve = Min(mPlayer->parts_[i]->dissolve, 2.f);
				}
			}
		});

	mPlayer->DebugUpdate();
	mPlayer->Update();
}
void Player::DrawModel()
{
	mPlayer->DrawModel();
}
void Player::DrawDebugGui()
{
	//static float power = 1;
	//static Color color = Color::red;
	//GuiManager::BeginWindow("Player Alpha");
	//GuiManager::DrawSlider1("D power", power, 0.01f);
	//GuiManager::DrawColorEdit("D color", color);
	//GuiManager::EndWindow();

	//for (uint32_t i = 1; i < mPlayer->parts_.size(); i++)
	//{
	//	mPlayer->parts_[i]->colorPower = power;
	//	mPlayer->parts_[i]->dissolveColor = color;
	//}

	mPlayer->DrawDebugGui();
}

void Player::Recovery()
{
	gaugePrames_[(uint32_t)GaugeType::Hp].value += 96.f;
	bottleNum_--;
	bottleNum_ = Max<uint32_t>(bottleNum_, 0);
}
void Player::Damage(const float damage)
{
	damageCoolTimer_.Reset();
	gaugePrames_[(uint32_t)GaugeType::Hp].value -= damage;

	if (gaugePrames_[(uint32_t)GaugeType::Hp].value <= 0)
	{
		isAlive_ = false;
	}
}

// コライダー関連
void Player::CalcFrontVec()
{
	// カメラの前ベクトル
	Vec3 cameForward = mPlayer->pos - Camera::current.pos;
	cameForward.y = 0.f;

	// カメラの右ベクトル
	Vec3 cameRight = Vec3::Cross(cameForward, Vec3::up);

	Vec3 stick =
	{
		Pad::GetStick(PadCode::LeftStick, 300).x,
		0,
		Pad::GetStick(PadCode::LeftStick, 300).y,
	};
	if (stick != 0)
	{
		Vec3 stickMoveVec = 0;

		stickMoveVec.x = -stick.Norm().x;
		stickMoveVec.z = -stick.Norm().z;

		frontVec_ = cameForward * stickMoveVec.z + cameRight * stickMoveVec.x;
	}
}
void Player::CalcBodyCollider()
{
	ColliderUpdate();
}
void Player::ColliderUpdate()
{
	bodyCollider_.startPos = mPlayer->pos - Vec3(0.f, 4.75f, 0.f);
	bodyCollider_.endPos = mPlayer->pos + Vec3(0.f, 4.75f, 0.f);
	bodyCollider_.radius = 2.5f;

	weapon_->ColliderUpdate();
}

// ゲージ関連
void Player::GaugeParamInit()
{
	// HPゲージ
	gaugePrames_[(uint32_t)GaugeType::Hp].CalcRate(256.f, 256.f);

	// MPゲージ
	gaugePrames_[(uint32_t)GaugeType::Mp].CalcRate(64.f, 64.f);

	// スタミナゲージ
	gaugePrames_[(uint32_t)GaugeType::Stamina].CalcRate(160.f, 160.f);
}
void Player::GaugeParamUpdate()
{
	if (state_ == State::Idle || state_ == State::Jogging || state_ == State::Drink)
	{
		gaugePrames_[(uint32_t)GaugeType::Stamina].value += 0.75f;
	}

	for (uint32_t i = 0; i < gaugePrames_.size(); i++)
	{
		gaugePrames_[i].Update();
	}
}

// ステート関連
void Player::MoveUpdate()
{
	CalcFrontVec();

	if (frontVec_ != 0)
	{
		moveVel = frontVec_.Norm() * moveSpeed_;

		mPlayer->pos += moveVel;
		mPlayer->rot.y = atan2f(frontVec_.x, frontVec_.z);
	}
}
void Player::IdleUpdate()
{
	mPlayer->IdleMotion();

	if (Pad::GetButtonDown(PadCode::ButtonR1))
	{
		if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 20 >= 0)
		{
			gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 20;
			state_ = State::AttackR1;
		}
	}
	else if (Pad::GetTrigger(PadCode::RightTrigger))
	{
		if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 40 >= 0)
		{
			gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 40;
			state_ = State::AttackR2;
		}
	}
	else if (Pad::GetButtonDown(PadCode::ButtonA))
	{
		if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
		{
			gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
			state_ = State::Backstep;
		}
	}
	else if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
	{
		if (Pad::GetButton(PadCode::ButtonA))
		{
			state_ = State::Run;
		}
		else
		{
			state_ = State::Jogging;
		}
	}
	else if (Pad::GetButtonDown(PadCode::ButtonX))
	{
		state_ = State::Drink;
	}
}
void Player::JoggingUpdate()
{
	mPlayer->JoggingMotion();

	MoveUpdate();

	if (Pad::GetButtonDown(PadCode::ButtonR1))
	{
		if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 20 >= 0)
		{
			gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 20;
			state_ = State::AttackR1;
		}
	}
	else if (Pad::GetTrigger(PadCode::RightTrigger))
	{
		if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 40 >= 0)
		{
			gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 40;
			state_ = State::AttackR2;
		}
	}
	else if (Pad::GetButton(PadCode::ButtonA))
	{
		// 何フレーム押したかを記録する
		pushTimer.Update();
		if (pushTimer == true)
		{
			state_ = State::Run;
			mPlayer->ChangeMoveMotionInit();
			pushTimer.Reset();
		}
	}
	else if (mPlayer->GetisPlayMoveMotion() == false)
	{
		state_ = State::Idle;
	}
	else if (Pad::GetButtonDown(PadCode::ButtonX))
	{
		state_ = State::Drink;
	}

	// 離した時
	if (Pad::GetButtonUp(PadCode::ButtonA))
	{
		if (pushTimer.GetisTimeOut() == false)
		{
			if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
			{
				gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
				state_ = State::Roll;

				mPlayer->RollMotionInit();
				mPlayer->ChangeMoveMotionInit();
			}
		}
		pushTimer.Reset();
	}
}
void Player::RunUpdate()
{
	mPlayer->RunMotion();

	gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 1.f;

	MoveUpdate();

	if (!Pad::GetButton(PadCode::ButtonA) ||
		gaugePrames_[(uint32_t)GaugeType::Stamina].value <= 0.f)
	{
		state_ = State::Jogging;
		mPlayer->ChangeMoveMotionInit();
	}
	else if (mPlayer->GetisPlayMoveMotion() == false)
	{
		state_ = State::Idle;
	}
	else if (Pad::GetButtonDown(PadCode::ButtonX))
	{
		state_ = State::Drink;
	}
}
void Player::BackstepUpdate()
{
	mPlayer->BackstepMotionUpdate();

	if (mPlayer->GetisBackStepMotionCanChange() == true)
	{
		if (Pad::GetButtonDown(PadCode::ButtonA))
		{
			if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
			{
				gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
				state_ = State::Backstep;
				mPlayer->BackstepMotionInit();
			}
		}
		else if (Pad::GetButtonDown(PadCode::ButtonR1))
		{
			if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 20 >= 0)
			{
				gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 20;
				mPlayer->BackstepMotionInit();
				state_ = State::AttackBack;
			}
		}
		else if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
		{
			mPlayer->BackstepMotionInit();
			mPlayer->ChangeMoveMotionInit();

			if (Pad::GetButton(PadCode::ButtonA))
			{
				if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
				{
					gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
					state_ = State::Roll;
				}
			}
			else
			{
				state_ = State::Jogging;
			}
		}
		else if (Pad::GetButtonDown(PadCode::ButtonX))
		{
			mPlayer->BackstepMotionInit();
			state_ = State::Drink;
		}
	}
	else if (mPlayer->GetisPlayBackStepMotion() == false)
	{
		state_ = State::Idle;
	}
}
void Player::RollUpdate()
{
	mPlayer->RollMotionUpdate();

	if (mPlayer->GetisRollMotionCanChange() == true)
	{
		if (Pad::GetButtonDown(PadCode::ButtonA))
		{
			if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
			{
				gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
				state_ = State::Backstep;
				mPlayer->RollMotionInit();
			}
		}
		else if (Pad::GetButtonDown(PadCode::ButtonR1))
		{
			if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 20 >= 0)
			{
				gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 20;
				state_ = State::AttackRoll;
				mPlayer->RollMotionInit();
			}
		}
		else if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
		{
			if (Pad::GetButtonDown(PadCode::ButtonA))
			{
				if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
				{
					gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
					state_ = State::Roll;
					mPlayer->RollMotionInit();
				}
			}
			else
			{
				state_ = State::Jogging;
				mPlayer->RollMotionInit();
			}
		}
		else if (Pad::GetButtonDown(PadCode::ButtonX))
		{
			state_ = State::Drink;
			mPlayer->RollMotionInit();
		}
	}

	if (mPlayer->GetisPlayRollMotion() == false)
	{
		if (state_ == State::Roll)
		{
			state_ = State::Idle;
			mPlayer->RollMotionInit();
		}
	}
}
void Player::AttackR1Update()
{
	mPlayer->AttackR1MotionUpdate();

	if (Pad::GetButtonDown(PadCode::ButtonA))
	{
		if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
		{
			if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
			{
				gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
				state_ = State::Roll;
				mPlayer->AttackMotionInit(WeaponPartID::Right);
			}
		}
		else
		{
			if (mPlayer->GetisAttackMotionCanChange(WeaponPartID::Right) == true)
			{
				if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
				{
					gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
					mPlayer->AttackMotionInit(WeaponPartID::Right);
					state_ = State::Backstep;
				}
			}
		}
	}

	if (mPlayer->GetisPlayAttackMotion(WeaponPartID::Right) == false)
	{
		if (state_ == State::AttackR1)
		{
			mPlayer->AttackMotionInit(WeaponPartID::Right);
			state_ = State::Idle;
		}
	}
}
void Player::AttackR2Update()
{
	mPlayer->AttackR2MotionUpdate();

	if (Pad::GetButtonDown(PadCode::ButtonA))
	{
		if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
		{
			if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
			{
				gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
				state_ = State::Roll;
				mPlayer->AttackMotionInit(WeaponPartID::Right);
			}
		}
		else
		{
			if (mPlayer->GetisAttackMotionCanChange(WeaponPartID::Right) == true)
			{
				if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
				{
					gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
					mPlayer->AttackMotionInit(WeaponPartID::Right);
					state_ = State::Backstep;
				}
			}
		}
	}

	if (mPlayer->GetisPlayAttackMotion(WeaponPartID::Right) == false)
	{
		if (state_ == State::AttackR2)
		{
			mPlayer->AttackMotionInit(WeaponPartID::Right);
			state_ = State::Idle;
		}
	}
}
void Player::AttackBackUpdate()
{
	mPlayer->AttackBackMotionUpdate();

	if (mPlayer->GetisPlayAttackMotion(WeaponPartID::Right) == false)
	{
		if (state_ == State::AttackBack)
		{
			mPlayer->AttackMotionInit(WeaponPartID::Right);
			state_ = State::Idle;
		}
	}
}
void Player::AttackRollUpdate()
{
	mPlayer->AttackRollMotionUpdate();

	if (mPlayer->GetisPlayAttackMotion(WeaponPartID::Right) == false)
	{
		if (state_ == State::AttackRoll)
		{
			mPlayer->AttackMotionInit(WeaponPartID::Right);
			state_ = State::Idle;
		}
	}
}
void Player::DrinkUpdate()
{
	auto motion = mPlayer->drinkMotion_.get();

	motion->Update(mPlayer.get());

	if (motion->GetisPlay() == false)
	{
		if (state_ == State::Drink)
		{
			motion->Init(mPlayer.get());
			state_ = State::Idle;
		}
	}
}

void Player::DamageUpdate()
{
	if (ismDamage == false)
	{
		return;
	}

	damageCoolTimer_.Update();
	if (damageCoolTimer_ == true)
	{
		damageCoolTimer_.Reset();
		ismDamage = false;
	}
}

// セッター
void Player::SetPos(const Vec3 pos)
{
	mPlayer->pos = pos;
}
void Player::SetisDamage(const bool isDamage)
{
	ismDamage = isDamage;
}

// ゲッター
GaugeParam Player::GetGaugeParam(const uint32_t index)
{
	return gaugePrames_[index];
}
PlayerBody* Player::GetPlayerBody()
{
	return mPlayer.get();
}
Weapon* Player::GetWeapon()
{
	return weapon_.get();
}
Vec3 Player::GetPos()
{
	return mPlayer->pos;
}
Vec3 Player::GetRot()
{
	return mPlayer->rot;
}
Vec3 Player::GetAttackPos()
{
	return mPlayer->attackPos;
}
Vec3 Player::GetHeadPos()
{
	return mPlayer->GetWorldPos(PartID::Head);
}
Vec3 Player::GetMoveVel()
{
	return moveVel;
}
Vec3 Player::GetFrontVec()
{
	return frontVec_.Norm();
}
Player::State Player::GetState()
{
	return state_;
}
CapsuleCollider Player::GetBodyCollider()
{
	return bodyCollider_;
}
bool Player::GetisDamage()
{
	return ismDamage;
}
bool Player::GetisAlive()
{
	return isAlive_;
}
bool Player::GetisDissolve()
{
	return isDissolve_;
}
uint32_t Player::GetBottleNum()
{
	return bottleNum_;
}
