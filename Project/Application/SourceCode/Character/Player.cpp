#include "Player.h"
#include "GaugeType.h"
#include "Club.h"
#include "Sword.h"

Player::Player() :
	mPlayer(std::make_unique<PlayerBody>()),
	mWeapon(std::make_unique<Club>()),
	mFrontVec(Vec3::front)
{
}

void Player::Init()
{
	GaugeParamInit();
	mPlayer->Init();
	mPlayer->pos = Vec3(42.f, 4.75f, 13.f);
	mPlayer->rot = Vec3(0, Radian(-90), 0);

	mPlayer->SetWeapon(mWeapon.get(), WeaponPartID::Right);
	mPlayer->iParent = this;
	mPlayer->parent = static_cast<Player*>(mPlayer->iParent);

	mState = State::Idle;

	mJoggingSpeed = 0.7f;
	mRunSpeed = 1.2f;

	mPushTimer.SetLimitTimer(20);
	mDamageCoolTimer.SetLimitTimer(30);

	mIsAlive = true;
	mIsDissolve = false;

	mWeapon->weapon->isUseDissolve = true;
	mWeapon->weapon->colorPower = 5;
	mWeapon->weapon->dissolveColor = Color(255, 30, 0, 255);

	mBottleNum = 4;

}
void Player::PrevUpdate()
{
	if (mMovieEvent->GetisPlaying() == true)
	{
		return;
	}

	mMoveVel = 0;

	if (mIsAlive == true)
	{
		// 髢｢謨ｰ繝昴う繝ｳ繧ｿ
		void (Player:: * pFunc[])() =
		{
			// 逋ｻ骭ｲ
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
			&Player::OpenGateUpdate,
		};

		// 螳溯｡・
		(this->*pFunc[(int)mState])();
	}

	GaugeParamUpdate();
	ColliderUpdate();
	DamageUpdate();

	if (mIsAlive == false)
	{
		for (uint32_t i = 1; i < mPlayer->mParts.size(); i++)
		{
			mPlayer->mParts[i]->dissolve += 0.005f;
			mPlayer->mParts[i]->dissolve = Min(mPlayer->mParts[i]->dissolve, 2.f);
		}

		mWeapon->weapon->dissolve += 0.005f;
		mWeapon->weapon->dissolve = Min(mWeapon->weapon->dissolve, 2.f);

		if (mPlayer->mParts[1]->dissolve >= 0.5f)
		{
			if (mIsDissolve == false)
			{
				mIsDissolve = true;
			}
		}
	}
}
void Player::PostUpdate()
{
	mPlayer->DebugUpdate();
	mPlayer->Update();
}
void Player::DrawModel()
{
	mPlayer->DrawModel();
}
void Player::DrawDebugGui()
{
	mPlayer->DrawDebugGui();
}

void Player::Recovery()
{
	mGaugePrames[(uint32_t)GaugeType::Hp].value += 96.f;
	mBottleNum--;
	mBottleNum = Max<uint32_t>(mBottleNum, 0);
}
void Player::Damage(const float damage)
{
	mDamageCoolTimer.Reset();
	mGaugePrames[(uint32_t)GaugeType::Hp].value -= damage;

	if (mGaugePrames[(uint32_t)GaugeType::Hp].value <= 0)
	{
		mIsAlive = false;
	}
}

// 繧ｳ繝ｩ繧､繝繝ｼ髢｢騾｣
void Player::CalcFrontVec()
{
	// 繧ｫ繝｡繝ｩ縺ｮ蜑阪・繧ｯ繝医Ν
	Vec3 cameForward = mPlayer->pos - Camera::current.pos;
	cameForward.y = 0.f;

	// 繧ｫ繝｡繝ｩ縺ｮ蜿ｳ繝吶け繝医Ν
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

		mFrontVec = cameForward * stickMoveVec.z + cameRight * stickMoveVec.x;
	}
}
void Player::CalcBodyCollider()
{
	ColliderUpdate();
}
void Player::ColliderUpdate()
{
	// 繝峨い繧帝幕縺代ｋ譎・
	if (mState == State::OpenGate)
	{
		mBodyCollider.isActive = false;
	}
	else
	{
		mBodyCollider.isActive = true;
	}

	mBodyCollider.startPos = mPlayer->pos - Vec3(0.f, 4.75f, 0.f);
	mBodyCollider.endPos = mPlayer->pos + Vec3(0.f, 4.75f, 0.f);
	mBodyCollider.radius = 2.5f;

	mWeapon->ColliderUpdate();
}

// 繧ｲ繝ｼ繧ｸ髢｢騾｣
void Player::GaugeParamInit()
{
	// HP繧ｲ繝ｼ繧ｸ
	mGaugePrames[(uint32_t)GaugeType::Hp].CalcRate(256.f, 256.f);

	// MP繧ｲ繝ｼ繧ｸ
	mGaugePrames[(uint32_t)GaugeType::Mp].CalcRate(64.f, 64.f);

	// 繧ｹ繧ｿ繝溘リ繧ｲ繝ｼ繧ｸ
	mGaugePrames[(uint32_t)GaugeType::Stamina].CalcRate(160.f, 160.f);
}
void Player::GaugeParamUpdate()
{
	if (mState == State::Idle || mState == State::Jogging || mState == State::Drink)
	{
		mGaugePrames[(uint32_t)GaugeType::Stamina].value += 0.75f;
	}

	for (uint32_t i = 0; i < mGaugePrames.size(); i++)
	{
		mGaugePrames[i].Update();
	}
}

// 繧ｹ繝・・繝磯未騾｣
void Player::MoveUpdate()
{
	CalcFrontVec();

	if (mFrontVec != 0)
	{
		mMoveVel = mFrontVec.Norm() * mMoveSpeed;

		mPlayer->pos += mMoveVel;
		mPlayer->rot.y = atan2f(mFrontVec.x, mFrontVec.z);
	}
}
void Player::IdleUpdate()
{
	mPlayer->IdleMotion();

	if (Pad::GetButtonDown(PadCode::ButtonR1))
	{
		if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 20 >= 0)
		{
			mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 20;
			mState = State::AttackR1;
		}
	}
	else if (Pad::GetTrigger(PadCode::RightTrigger))
	{
		if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 40 >= 0)
		{
			mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 40;
			mState = State::AttackR2;
		}
	}
	else if (Pad::GetButtonDown(PadCode::ButtonA))
	{
		if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
		{
			mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 30;
			mState = State::Backstep;
		}
	}
	else if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
	{
		if (Pad::GetButton(PadCode::ButtonA))
		{
			mState = State::Run;
		}
		else
		{
			mState = State::Jogging;
		}
	}
	else if (Pad::GetButtonDown(PadCode::ButtonX))
	{
		mState = State::Drink;
	}
	else if (Key::GetKeyDown(DIK_SPACE))
	{
		mState = State::OpenGate;
	}
}
void Player::JoggingUpdate()
{
	mPlayer->JoggingMotion();

	MoveUpdate();

	if (Pad::GetButtonDown(PadCode::ButtonR1))
	{
		if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 20 >= 0)
		{
			mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 20;
			mState = State::AttackR1;
		}
	}
	else if (Pad::GetTrigger(PadCode::RightTrigger))
	{
		if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 40 >= 0)
		{
			mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 40;
			mState = State::AttackR2;
		}
	}
	else if (Pad::GetButton(PadCode::ButtonA))
	{
		// 菴輔ヵ繝ｬ繝ｼ繝謚ｼ縺励◆縺九ｒ險倬鹸縺吶ｋ
		mPushTimer.Update();
		if (mPushTimer == true)
		{
			mState = State::Run;
			mPlayer->ChangeMoveMotionInit();
			mPushTimer.Reset();
		}
	}
	else if (mPlayer->GetisPlayMoveMotion() == false)
	{
		mState = State::Idle;
	}
	else if (Pad::GetButtonDown(PadCode::ButtonX))
	{
		mState = State::Drink;
	}

	// 髮｢縺励◆譎・
	if (Pad::GetButtonUp(PadCode::ButtonA))
	{
		if (mPushTimer.GetisTimeOut() == false)
		{
			if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
			{
				mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 30;
				mState = State::Roll;

				mPlayer->RollMotionInit();
				mPlayer->ChangeMoveMotionInit();
			}
		}
		mPushTimer.Reset();
	}
}
void Player::RunUpdate()
{
	mPlayer->RunMotion();

	mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 1.f;

	MoveUpdate();

	if (!Pad::GetButton(PadCode::ButtonA) ||
		mGaugePrames[(uint32_t)GaugeType::Stamina].value <= 0.f)
	{
		mState = State::Jogging;
		mPlayer->ChangeMoveMotionInit();
	}
	else if (mPlayer->GetisPlayMoveMotion() == false)
	{
		mState = State::Idle;
	}
	else if (Pad::GetButtonDown(PadCode::ButtonX))
	{
		mState = State::Drink;
	}
}
void Player::BackstepUpdate()
{
	mPlayer->BackstepMotionUpdate();

	if (mPlayer->GetisBackStepMotionCanChange() == true)
	{
		if (Pad::GetButtonDown(PadCode::ButtonA))
		{
			if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
			{
				mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 30;
				mState = State::Backstep;
				mPlayer->BackstepMotionInit();
			}
		}
		else if (Pad::GetButtonDown(PadCode::ButtonR1))
		{
			if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 20 >= 0)
			{
				mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 20;
				mPlayer->BackstepMotionInit();
				mState = State::AttackBack;
			}
		}
		else if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
		{
			mPlayer->BackstepMotionInit();
			mPlayer->ChangeMoveMotionInit();

			if (Pad::GetButton(PadCode::ButtonA))
			{
				if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
				{
					mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 30;
					mState = State::Roll;
				}
			}
			else
			{
				mState = State::Jogging;
			}
		}
		else if (Pad::GetButtonDown(PadCode::ButtonX))
		{
			mPlayer->BackstepMotionInit();
			mState = State::Drink;
		}
	}
	else if (mPlayer->GetisPlayBackStepMotion() == false)
	{
		mState = State::Idle;
	}
}
void Player::RollUpdate()
{
	mPlayer->RollMotionUpdate();

	if (mPlayer->GetisRollMotionCanChange() == true)
	{
		if (Pad::GetButtonDown(PadCode::ButtonA))
		{
			if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
			{
				mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 30;
				mState = State::Backstep;
				mPlayer->RollMotionInit();
			}
		}
		else if (Pad::GetButtonDown(PadCode::ButtonR1))
		{
			if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 20 >= 0)
			{
				mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 20;
				mState = State::AttackRoll;
				mPlayer->RollMotionInit();
			}
		}
		else if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
		{
			if (Pad::GetButtonDown(PadCode::ButtonA))
			{
				if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
				{
					mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 30;
					mState = State::Roll;
					mPlayer->RollMotionInit();
				}
			}
			else
			{
				mState = State::Jogging;
				mPlayer->RollMotionInit();
			}
		}
		else if (Pad::GetButtonDown(PadCode::ButtonX))
		{
			mState = State::Drink;
			mPlayer->RollMotionInit();
		}
	}

	if (mPlayer->GetisPlayRollMotion() == false)
	{
		if (mState == State::Roll)
		{
			mState = State::Idle;
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
			if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
			{
				mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 30;
				mState = State::Roll;
				mPlayer->AttackMotionInit(WeaponPartID::Right);
			}
		}
		else
		{
			if (mPlayer->GetisAttackMotionCanChange(WeaponPartID::Right) == true)
			{
				if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
				{
					mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 30;
					mPlayer->AttackMotionInit(WeaponPartID::Right);
					mState = State::Backstep;
				}
			}
		}
	}

	if (mPlayer->GetisPlayAttackMotion(WeaponPartID::Right) == false)
	{
		if (mState == State::AttackR1)
		{
			mPlayer->AttackMotionInit(WeaponPartID::Right);
			mState = State::Idle;
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
			if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
			{
				mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 30;
				mState = State::Roll;
				mPlayer->AttackMotionInit(WeaponPartID::Right);
			}
		}
		else
		{
			if (mPlayer->GetisAttackMotionCanChange(WeaponPartID::Right) == true)
			{
				if (mGaugePrames[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
				{
					mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 30;
					mPlayer->AttackMotionInit(WeaponPartID::Right);
					mState = State::Backstep;
				}
			}
		}
	}

	if (mPlayer->GetisPlayAttackMotion(WeaponPartID::Right) == false)
	{
		if (mState == State::AttackR2)
		{
			mPlayer->AttackMotionInit(WeaponPartID::Right);
			mState = State::Idle;
		}
	}
}
void Player::AttackBackUpdate()
{
	mPlayer->AttackBackMotionUpdate();

	if (mPlayer->GetisPlayAttackMotion(WeaponPartID::Right) == false)
	{
		if (mState == State::AttackBack)
		{
			mPlayer->AttackMotionInit(WeaponPartID::Right);
			mState = State::Idle;
		}
	}
}
void Player::AttackRollUpdate()
{
	mPlayer->AttackRollMotionUpdate();

	if (mPlayer->GetisPlayAttackMotion(WeaponPartID::Right) == false)
	{
		if (mState == State::AttackRoll)
		{
			mPlayer->AttackMotionInit(WeaponPartID::Right);
			mState = State::Idle;
		}
	}
}
void Player::DrinkUpdate()
{
	auto motion = mPlayer->mDrinkMotion.get();

	motion->Update(mPlayer.get());

	if (motion->GetisPlay() == false)
	{
		if (mState == State::Drink)
		{
			motion->Init(mPlayer.get());
			mState = State::Idle;
		}
	}
}
void Player::OpenGateUpdate()
{
	auto motion = mPlayer->mOpenGateMotion.get();

	motion->Update(mPlayer.get());

	if (motion->GetisPlay() == false)
	{
		if (mState == State::OpenGate)
		{
			motion->Init(mPlayer.get());
			mState = State::Idle;
		}
	}
}

void Player::DamageUpdate()
{
	if (mIsDamage == false)
	{
		return;
	}

	mDamageCoolTimer.Update();
	if (mDamageCoolTimer == true)
	{
		mDamageCoolTimer.Reset();
		mIsDamage = false;
	}
}

void Player::SetMovieEvent(MovieEvent* movieEvent)
{
	mMovieEvent = movieEvent;
}

// 繧ｻ繝・ち繝ｼ
void Player::SetPos(const Vec3 pos)
{
	mPlayer->pos = pos;
}
void Player::SetRot(const Vec3 rot)
{
	mPlayer->rot = rot;
}
void Player::SetisDamage(const bool isDamage)
{
	mIsDamage = isDamage;
}
void Player::SetState(const State state)
{
	mState = state;
}
void Player::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	for (uint32_t i = 0; i < mPlayer->mParts.size(); i++)
	{
		mPlayer->mParts[i]->SetGraphicsPipeline(graphicsPipeline);
	}
	mWeapon->SetGraphicsPipeline(graphicsPipeline);
}

// 繧ｲ繝・ち繝ｼ
GaugeParam Player::GetGaugeParam(const uint32_t index)
{
	return mGaugePrames[index];
}
PlayerBody* Player::GetPlayerBody()
{
	return mPlayer.get();
}
Weapon* Player::GetWeapon()
{
	return mWeapon.get();
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
	return mMoveVel;
}
Vec3 Player::GetFrontVec()
{
	return mFrontVec.Norm();
}
Player::State Player::GetState()
{
	return mState;
}
CapsuleCollider Player::GetBodyCollider()
{
	return mBodyCollider;
}
bool Player::GetisDamage()
{
	return mIsDamage;
}
bool Player::GetisAlive()
{
	return mIsAlive;
}
bool Player::GetisDissolve()
{
	return mIsDissolve;
}
uint32_t Player::GetBottleNum()
{
	return mBottleNum;
}

