#include "CollisionManager.h"

void CollisionManager::PlayerHitBoss()
{
	bool isAttackBoss =
		Collision::CapsuleHitCapsule(
			player_->GetWeapon()->collider,
			boss_->GetCollider());

	if (isAttackBoss)
	{
		if (player_->GetState() == Player::State::AttackR1)
		{
			boss_->Damage(10.f);
		}
	}
}

void CollisionManager::Update()
{
	PlayerHitBoss();
}

void CollisionManager::SetPlayer(Player* player)
{
	player_ = player;
}

void CollisionManager::SetBoss(Boss* boss)
{
	boss_ = boss;
}
