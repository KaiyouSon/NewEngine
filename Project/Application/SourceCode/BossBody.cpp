#include "BossBody.h"
#include "Boss.h"

BossBody::BossBody() :
	grabAttackMotion_(std::make_unique<BossGrabAttackMotion>()),
	attack2Motion_(std::make_unique<BossAttack2Motion>()),
	attack3Motion_(std::make_unique<BossAttack3Motion>())
{
	//
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
	grabAttackMotion_->Update(this);
	//attack2Motion_->Update(this);
	//attack3Motion_->Update(this);
}

void BossBody::DrawModel()
{
	BaseDrawModel();
}
