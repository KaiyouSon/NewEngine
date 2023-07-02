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
	Vec2 stick = Pad::GetStick(PadCode::LeftStick);
	if (stick.x > 300 || stick.x < -300 || stick.y >= 300 || stick.y <= -300)
	{
		player_->JoggingMotion();
	}
	else
	{
		player_->IdleMotion();
	}

	MoveUpdate();
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
	if (stick.x > 300 || stick.x < -300 || stick.z >300 || stick.z < -300)
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
		player_->pos += frontVec.Norm() * 1.f;
		player_->rot.y = atan2f(frontVec.Norm().x, frontVec.Norm().z);
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
