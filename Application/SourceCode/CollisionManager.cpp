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

	bool isBodyTouch =
		Collision::CubeHitCube(
			player_->GetHumanoidBody()->GetBodyCollider(),
			boss_->GetBodyCollider());

	if (isBodyTouch == true)
	{
		Vec3 pos = player_->GetPos();
		while (isBodyTouch)
		{
			isBodyTouch =
				Collision::CubeHitCube(
					player_->GetHumanoidBody()->GetBodyCollider(),
					boss_->GetBodyCollider());

			const float check = 0.01f;
			Vec3 v = pos - boss_->GetPos();
			Vec3 nextPos = player_->GetPos() + v.Norm() * check;
			player_->SetPos(nextPos);

			player_->CalcBodyCollider();
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
