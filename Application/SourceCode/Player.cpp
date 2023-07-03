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
}
void Player::PrevUpdate()
{
	player_->vel = 0;

	// 関数ポインタ
	void (Player:: * pFunc[])() =
	{
		// 登録
		&Player::IdleUpdate,
		&Player::JoggingUpdate,
		&Player::AttackR1Update,
	};

	// 実行
	(this->*pFunc[(int)state_])();

	player_->pos.y = 4.5f;

	player_->PrevUpdate();
	//player_->PostUpdate();
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

void Player::MoveUpdate()
{
	player_->CalcFrontVec();

	if (player_->frontVec != 0)
	{
		player_->vel = player_->frontVec.Norm() * moveSpeed;

		player_->pos += player_->vel;
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
		state_ = State::Jogging;
	}
}
void Player::JoggingUpdate()
{
	player_->JoggingMotion();

	moveSpeed = 0.75f;
	MoveUpdate();

	if (Pad::GetButtonDown(PadCode::ButtonR1))
	{
		state_ = State::AttackR1;
	}
	else if (Pad::GetStick(PadCode::LeftStick, 300) == 0)
	{
		state_ = State::Idle;
	}
}
void Player::RunUpdate()
{
	player_->JoggingMotion();

	moveSpeed = 2.f;
	MoveUpdate();

	if (Pad::GetStick(PadCode::LeftStick, 300) == 0)
	{
		state_ = State::Idle;
	}
}
void Player::AttackR1Update()
{
	player_->AttackMotion();

	if (player_->GetisPlayAttackMotion(0) == false)
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
Vec3 Player::GetVel()
{
	return player_->vel;
}
Player::State Player::GetState()
{
	return state_;
}
