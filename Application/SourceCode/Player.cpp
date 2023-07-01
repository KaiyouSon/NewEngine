#include "Player.h"
#include "GaugeType.h"

Player::Player() :
	player_(std::make_unique<HumanoidBody>())
{
}

void Player::Init()
{
	GaugeParamInit();
	player_->Init();
}

void Player::Update()
{
	Vec2 stick = Pad::GetStick(PadCode::LeftStick);
	if (stick.y >= 300 || stick.y <= -300)
	{
		player_->JoggingMotion();
	}
	else
	{
		player_->IdleMotion();
	}
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

GaugeParam Player::GetGaugeParam(const uint32_t index)
{
	return gaugePrames_[index];
}