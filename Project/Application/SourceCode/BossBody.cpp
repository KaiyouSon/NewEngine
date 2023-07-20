#include "BossBody.h"
#include "Boss.h"

BossBody::BossBody() :
	attack2Motion_(std::make_unique<BossAttack2Motion>())
{
	//
}

void BossBody::Init()
{
	BaseInit();
	attack2Motion_->Init(this);
}

void BossBody::Update()
{
	BaseUpdate();
	//attack2Motion_->Update(this);
}

void BossBody::DrawModel()
{
	BaseDrawModel();
}
