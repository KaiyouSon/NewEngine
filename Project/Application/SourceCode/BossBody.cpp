#include "BossBody.h"
#include "Boss.h"

BossBody::BossBody() :
	grabAttackMotion_(std::make_unique<BossGrabAttackMotion>()),
	attack2Motion_(std::make_unique<BossAttack2Motion>()),
	attack3Motion_(std::make_unique<BossAttack3Motion>())
{
}

void BossBody::Init()
{
	BaseInit();
	grabAttackMotion_->Init(this);
	attack2Motion_->Init(this);
	attack3Motion_->Init(this);
}

void BossBody::Update()
{
	BaseUpdate();
}

void BossBody::DrawModel()
{
	BaseDrawModel();
}

void BossBody::GrabAttackMotion()
{
	grabAttackMotion_->Update(this);
	isPlayMotion_ = grabAttackMotion_->GetisPlay();
}
void BossBody::Attack2Motion()
{
	attack2Motion_->Update(this);
	isPlayMotion_ = attack2Motion_->GetisPlay();
}
void BossBody::Attack3Motion()
{
	attack3Motion_->Update(this);
	isPlayMotion_ = attack3Motion_->GetisPlay();
}

bool BossBody::GetisPlayMotion()
{
	return isPlayMotion_;
}
