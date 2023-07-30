#include "Player.h"
#include "GaugeType.h"
#include "Club.h"
#include "Sword.h"

Player::Player() :
	player_(std::make_unique<PlayerBody>()),
	weapon_(std::make_unique<Club>()),
	frontVec_(Vec3::front)
{
}

void Player::Init()
{
	GaugeParamInit();
	player_->Init();
	player_->pos.y = 4.75f;

	player_->SetWeapon(weapon_.get(), WeaponPartID::Right);
	player_->iParent = this;
	player_->parent = static_cast<Player*>(player_->iParent);

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
		for (uint32_t i = 1; i < player_->parts_.size(); i++)
		{
			player_->parts_[i]->dissolve += 0.005f;
			player_->parts_[i]->dissolve = Min(player_->parts_[i]->dissolve, 2.f);
		}

		weapon_->weapon->dissolve += 0.005f;
		weapon_->weapon->dissolve = Min(weapon_->weapon->dissolve, 2.f);

		if (player_->parts_[1]->dissolve >= 0.5f)
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
				for (uint32_t i = 0; i < player_->parts_.size(); i++)
				{
					player_->parts_[i]->dissolve = 0.f;
				}

			}
			if (flag == true)
			{
				for (uint32_t i = 0; i < player_->parts_.size(); i++)
				{
					player_->parts_[i]->dissolve += 0.005f;
					player_->parts_[i]->dissolve = Min(player_->parts_[i]->dissolve, 2.f);
				}
			}
		});

	player_->DebugUpdate();
	player_->Update();
}
void Player::DrawModel()
{
	player_->DrawModel();
}
void Player::DrawDebugGui()
{
	//static float power = 1;
	//static Color color = Color::red;
	//GuiManager::BeginWindow("Player Alpha");
	//GuiManager::DrawSlider1("D power", power, 0.01f);
	//GuiManager::DrawColorEdit("D color", color);
	//GuiManager::EndWindow();

	//for (uint32_t i = 1; i < player_->parts_.size(); i++)
	//{
	//	player_->parts_[i]->colorPower = power;
	//	player_->parts_[i]->dissolveColor = color;
	//}

	player_->DrawDebugGui();
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
	Vec3 cameForward = player_->pos - Camera::current.pos;
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
	bodyCollider_.startPos = player_->pos - Vec3(0.f, 4.75f, 0.f);
	bodyCollider_.endPos = player_->pos + Vec3(0.f, 4.75f, 0.f);
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

		player_->pos += moveVel;
		player_->rot.y = atan2f(frontVec_.x, frontVec_.z);
	}
}
void Player::IdleUpdate()
{
	player_->IdleMotion();

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
	player_->JoggingMotion();

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
		pushTimer.Update(false);
		if (pushTimer.GetisTimeOut() == true)
		{
			state_ = State::Run;
			player_->ChangeMoveMotionInit();
			pushTimer.Reset();
		}
	}
	else if (player_->GetisPlayMoveMotion() == false)
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

				player_->RollMotionInit();
				player_->ChangeMoveMotionInit();
			}
		}
		pushTimer.Reset();
	}
}
void Player::RunUpdate()
{
	player_->RunMotion();

	gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 1.f;

	MoveUpdate();

	if (!Pad::GetButton(PadCode::ButtonA) ||
		gaugePrames_[(uint32_t)GaugeType::Stamina].value <= 0.f)
	{
		state_ = State::Jogging;
		player_->ChangeMoveMotionInit();
	}
	else if (player_->GetisPlayMoveMotion() == false)
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
	player_->BackstepMotionUpdate();

	if (player_->GetisBackStepMotionCanChange() == true)
	{
		if (Pad::GetButtonDown(PadCode::ButtonA))
		{
			if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
			{
				gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
				state_ = State::Backstep;
				player_->BackstepMotionInit();
			}
		}
		else if (Pad::GetButtonDown(PadCode::ButtonR1))
		{
			if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 20 >= 0)
			{
				gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 20;
				player_->BackstepMotionInit();
				state_ = State::AttackBack;
			}
		}
		else if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
		{
			player_->BackstepMotionInit();
			player_->ChangeMoveMotionInit();

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
			player_->BackstepMotionInit();
			state_ = State::Drink;
		}
	}
	else if (player_->GetisPlayBackStepMotion() == false)
	{
		state_ = State::Idle;
	}
}
void Player::RollUpdate()
{
	player_->RollMotionUpdate();

	if (player_->GetisRollMotionCanChange() == true)
	{
		if (Pad::GetButtonDown(PadCode::ButtonA))
		{
			if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
			{
				gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
				state_ = State::Backstep;
				player_->RollMotionInit();
			}
		}
		else if (Pad::GetButtonDown(PadCode::ButtonR1))
		{
			if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 20 >= 0)
			{
				gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 20;
				state_ = State::AttackRoll;
				player_->RollMotionInit();
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
					player_->RollMotionInit();
				}
			}
			else
			{
				state_ = State::Jogging;
				player_->RollMotionInit();
			}
		}
		else if (Pad::GetButtonDown(PadCode::ButtonX))
		{
			state_ = State::Drink;
			player_->RollMotionInit();
		}
	}

	if (player_->GetisPlayRollMotion() == false)
	{
		if (state_ == State::Roll)
		{
			state_ = State::Idle;
			player_->RollMotionInit();
		}
	}
}
void Player::AttackR1Update()
{
	player_->AttackR1MotionUpdate();

	if (Pad::GetButtonDown(PadCode::ButtonA))
	{
		if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
		{
			if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
			{
				gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
				state_ = State::Roll;
				player_->AttackMotionInit(WeaponPartID::Right);
			}
		}
		else
		{
			if (player_->GetisAttackMotionCanChange(WeaponPartID::Right) == true)
			{
				if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
				{
					gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
					player_->AttackMotionInit(WeaponPartID::Right);
					state_ = State::Backstep;
				}
			}
		}
	}

	if (player_->GetisPlayAttackMotion(WeaponPartID::Right) == false)
	{
		if (state_ == State::AttackR1)
		{
			player_->AttackMotionInit(WeaponPartID::Right);
			state_ = State::Idle;
		}
	}
}
void Player::AttackR2Update()
{
	player_->AttackR2MotionUpdate();

	if (Pad::GetButtonDown(PadCode::ButtonA))
	{
		if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
		{
			if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
			{
				gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
				state_ = State::Roll;
				player_->AttackMotionInit(WeaponPartID::Right);
			}
		}
		else
		{
			if (player_->GetisAttackMotionCanChange(WeaponPartID::Right) == true)
			{
				if (gaugePrames_[(uint32_t)GaugeType::Stamina].value - 30 >= 0)
				{
					gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 30;
					player_->AttackMotionInit(WeaponPartID::Right);
					state_ = State::Backstep;
				}
			}
		}
	}

	if (player_->GetisPlayAttackMotion(WeaponPartID::Right) == false)
	{
		if (state_ == State::AttackR2)
		{
			player_->AttackMotionInit(WeaponPartID::Right);
			state_ = State::Idle;
		}
	}
}
void Player::AttackBackUpdate()
{
	player_->AttackBackMotionUpdate();

	if (player_->GetisPlayAttackMotion(WeaponPartID::Right) == false)
	{
		if (state_ == State::AttackBack)
		{
			player_->AttackMotionInit(WeaponPartID::Right);
			state_ = State::Idle;
		}
	}
}
void Player::AttackRollUpdate()
{
	player_->AttackRollMotionUpdate();

	if (player_->GetisPlayAttackMotion(WeaponPartID::Right) == false)
	{
		if (state_ == State::AttackRoll)
		{
			player_->AttackMotionInit(WeaponPartID::Right);
			state_ = State::Idle;
		}
	}
}
void Player::DrinkUpdate()
{
	auto motion = player_->drinkMotion_.get();

	motion->Update(player_.get());

	if (motion->GetisPlay() == false)
	{
		if (state_ == State::Drink)
		{
			motion->Init(player_.get());
			state_ = State::Idle;
		}
	}
}

void Player::DamageUpdate()
{
	if (isDamage_ == false)
	{
		return;
	}

	damageCoolTimer_.Update(false);
	if (damageCoolTimer_.GetisTimeOut() == true)
	{
		damageCoolTimer_.Reset();
		isDamage_ = false;
	}
}

// セッター
void Player::SetPos(const Vec3 pos)
{
	player_->pos = pos;
}
void Player::SetisDamage(const bool isDamage)
{
	isDamage_ = isDamage;
}

// ゲッター
GaugeParam Player::GetGaugeParam(const uint32_t index)
{
	return gaugePrames_[index];
}
PlayerBody* Player::GetPlayerBody()
{
	return player_.get();
}
Weapon* Player::GetWeapon()
{
	return weapon_.get();
}
Vec3 Player::GetPos()
{
	return player_->pos;
}
Vec3 Player::GetRot()
{
	return player_->rot;
}
Vec3 Player::GetAttackPos()
{
	return player_->attackPos;
}
Vec3 Player::GetHeadPos()
{
	return player_->GetWorldPos(PartID::Head);
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
	return isDamage_;
}
bool Player::GetisAlive()
{
	return isAlive_;
}
bool Player::GetisDissolve()
{
	return isDissolve_;
}
uint32_t Player::GetButtonNum()
{
	return bottleNum_;
}
