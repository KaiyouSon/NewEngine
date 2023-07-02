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
void Player::Update()
{
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

	player_->Update();
}
void Player::DrawModel()
{
	player_->DrawModel();
}
void Player::DrawDebugGui()
{
	player_->DrawDebugGui();
}

void Player::GaugeParamInit()
{
	// HPゲージ
	gaugePrames_[(uint32_t)GaugeType::Hp].max = 256.f;
	gaugePrames_[(uint32_t)GaugeType::Hp].value = gaugePrames_[(uint32_t)GaugeType::Hp].max;
	gaugePrames_[(uint32_t)GaugeType::Hp].rate =
		gaugePrames_[(uint32_t)GaugeType::Hp].value /
		gaugePrames_[(uint32_t)GaugeType::Hp].max;

	// MPゲージ
	gaugePrames_[(uint32_t)GaugeType::Mp].max = 64.f;
	gaugePrames_[(uint32_t)GaugeType::Mp].value = gaugePrames_[(uint32_t)GaugeType::Mp].max;
	gaugePrames_[(uint32_t)GaugeType::Mp].rate =
		gaugePrames_[(uint32_t)GaugeType::Mp].value /
		gaugePrames_[(uint32_t)GaugeType::Mp].max;

	// スタミナゲージ
	gaugePrames_[(uint32_t)GaugeType::Stamina].max = 128.f;
	gaugePrames_[(uint32_t)GaugeType::Stamina].value = gaugePrames_[(uint32_t)GaugeType::Stamina].max;
	gaugePrames_[(uint32_t)GaugeType::Stamina].rate =
		gaugePrames_[(uint32_t)GaugeType::Stamina].value /
		gaugePrames_[(uint32_t)GaugeType::Stamina].max;
}

void Player::MoveUpdate()
{
	Vec3 moveVec = 0;

	// カメラの前ベクトル
	Vec3 cameForward = player_->pos - Camera::current.pos;
	cameForward.y = 0.f;

	// カメラの右ベクトル
	Vec3 cameRight = Vec3::Cross(cameForward, Vec3::up);

	Vec3 stick =
	{
		Pad::GetStick(PadCode::LeftStick).x,
		0,
		Pad::GetStick(PadCode::LeftStick).y,
	};
	if (stick != 0)
	{
		moveVec.x = -stick.Norm().x;
		moveVec.z = -stick.Norm().z;

		frontVec = cameForward * moveVec.z + cameRight * moveVec.x;
	}
	else
	{
		frontVec = 0;
	}

	if (frontVec != 0)
	{
		player_->pos += frontVec.Norm() * moveSpeed;
		player_->rot.y = atan2f(frontVec.Norm().x, frontVec.Norm().z);
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

	moveSpeed = 1.f;
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

GaugeParam Player::GetGaugeParam(const uint32_t index)
{
	return gaugePrames_[index];
}

Vec3 Player::GetHeadPos()
{
	return player_->GetWorldPos(PartID::Head);
}
