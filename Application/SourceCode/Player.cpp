#include "Player.h"
#include "GaugeType.h"
#include "Club.h"

Player::Player() :
	player_(std::make_unique<HumanoidBody>()),
	weapon_(std::make_unique<Club>())
{
}

void Player::Init()
{
	GaugeParamInit();
	player_->Init();
	player_->pos.y = 4.5f;

	player_->SetWeapon(weapon_.get(), 0);
	player_->parent = this;

	joggingSpeed_ = 0.7f;
	runSpeed_ = 1.2f;

	pushTimer.SetLimitTimer(20);
}
void Player::PrevUpdate()
{
	moveVel = 0;

	// 関数ポインタ
	void (Player:: * pFunc[])() =
	{
		// 登録
		&Player::IdleUpdate,
		&Player::JoggingUpdate,
		&Player::RunUpdate,
		&Player::AttackR1Update,
		&Player::BackstepUpdate,
	};

	// 実行
	(this->*pFunc[(int)state_])();

	GaugeParamUpdate();

	//player_->pos.y = 4.5f;
	player_->PrevUpdate();
}
void Player::PostUpdate()
{
	player_->PostUpdate();
}
void Player::DrawModel()
{
	player_->DrawModel();
}
void Player::DrawDebugGui()
{
	player_->DrawDebugGui();
}

void Player::CalcBodyCollider()
{
	player_->ColliderUpdate();
}

void Player::GaugeParamInit()
{
	// HPゲージ
	gaugePrames_[(uint32_t)GaugeType::Hp].CalcRate(256.f, 256.f);

	// MPゲージ
	gaugePrames_[(uint32_t)GaugeType::Mp].CalcRate(64.f, 64.f);

	// スタミナゲージ
	gaugePrames_[(uint32_t)GaugeType::Stamina].CalcRate(128.f, 128.f);
}
void Player::GaugeParamUpdate()
{
	if (state_ != State::AttackR1 && state_ != State::Run)
	{
		gaugePrames_[(uint32_t)GaugeType::Stamina].value++;
	}

	for (uint32_t i = 0; i < gaugePrames_.size(); i++)
	{
		gaugePrames_[i].Update();
	}
}

void Player::MoveUpdate()
{
	player_->CalcFrontVec();

	if (player_->frontVec != 0)
	{
		moveVel = player_->frontVec.Norm() * moveSpeed_;

		player_->pos += moveVel;
		player_->rot.y = atan2f(player_->frontVec.x, player_->frontVec.z);
	}
}
void Player::IdleUpdate()
{
	player_->IdleMotion();

	if (Pad::GetButtonDown(PadCode::ButtonR1))
	{
		state_ = State::AttackR1;
	}
	else if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
	{
		if (Pad::GetButton(PadCode::ButtonB))
		{
			state_ = State::Run;
		}
		else
		{
			state_ = State::Jogging;
		}
	}
	else if (Pad::GetButtonDown(PadCode::ButtonB))
	{
		state_ = State::Backstep;
	}
}
void Player::JoggingUpdate()
{
	player_->JoggingMotion();

	MoveUpdate();

	if (Pad::GetButtonDown(PadCode::ButtonR1))
	{
		state_ = State::AttackR1;
	}
	else if (Pad::GetButton(PadCode::ButtonB))
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

	// 離した時
	if (Pad::GetButtonUp(PadCode::ButtonB))
	{
		pushTimer.Reset();
	}
}
void Player::RunUpdate()
{
	player_->RunMotion();

	gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 1.f;

	MoveUpdate();

	if (!Pad::GetButton(PadCode::ButtonB) ||
		gaugePrames_[(uint32_t)GaugeType::Stamina].value <= 0.f)
	{
		state_ = State::Jogging;
		player_->ChangeMoveMotionInit();
	}
	else if (player_->GetisPlayMoveMotion() == false)
	{
		state_ = State::Idle;
	}
}
void Player::AttackR1Update()
{
	player_->AttackMotion();

	if (player_->GetisAttackMotionCanChange(0) == true)
	{
		if (Pad::GetButtonDown(PadCode::ButtonB))
		{
			player_->AttackMotionInit(0);
			state_ = State::Backstep;
		}
	}

	if (player_->GetisPlayAttackMotion(0) == false)
	{
		state_ = State::Idle;
	}

}
void Player::BackstepUpdate()
{
	player_->BackstepMotionUpdate();

	if (player_->GetisPlayBackStepMotion() == false)
	{
		state_ = State::Idle;
	}
}

void Player::SetPos(const Vec3 pos)
{
	player_->pos = pos;
}

GaugeParam Player::GetGaugeParam(const uint32_t index)
{
	return gaugePrames_[index];
}
HumanoidBody* Player::GetHumanoidBody()
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
	return player_->frontVec;
}
Player::State Player::GetState()
{
	return state_;
}
