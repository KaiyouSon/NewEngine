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
		Collision::CapsuleHitCapsule(
			player_->GetHumanoidBody()->GetBodyCollider(),
			boss_->GetCollider());

	if (isBodyTouch == true)
	{
		// 半径を足して実際の長さを求める
		float checkLength =
			player_->GetHumanoidBody()->GetBodyCollider().radius +
			boss_->GetCollider().radius;

		// y軸を無視する
		Vec3 pos1 = player_->GetPos() * Vec3(1, 0, 1);
		Vec3 pos2 = boss_->GetPos() * Vec3(1, 0, 1);

		Vec3 toPlayer = pos1 - pos2;

		// 現在の長さ
		float curLength = toPlayer.Length();

		// checkLength と curLength の差分で埋め込んだ長さが求まる
		float embedLength = checkLength - curLength;

		// 誤差をけす処理
		Vec3 normal = toPlayer.Norm();
		if (fabs(curLength) < 0.0001f)
		{
			normal = 0;
		}

		Vec3 pushVec = normal * embedLength;

		Vec3 nextPos = player_->GetPos() + pushVec;
		player_->SetPos(nextPos);
	}
}

void CollisionManager::Update()
{
	PlayerHitBoss();
}

bool CollisionManager::IsCheckFrontBoss(const Vec3 pos, const Vec3 front)
{
	SphereCollider frontCollider;
	frontCollider.centerPos = pos + front;
	frontCollider.radius = 1.0f;

	SphereCollider bossCollider;
	bossCollider.centerPos = boss_->GetPos();
	bossCollider.radius = 1.0f;

	bool isHit = Collision::SphereHitSphere(frontCollider, bossCollider);
	return isHit;
}

bool CollisionManager::IsCheckPlayerMove(float* dis)
{
	// 前ベクトルの計算
	player_->GetHumanoidBody()->CalcFrontVec();

	// y軸無視
	Vec3 offset = player_->GetHumanoidBody()->frontVec * *dis;

	SphereCollider playerFront;
	playerFront.centerPos = player_->GetPos();
	playerFront.radius = *dis;

	SphereCollider bossCollider;
	bossCollider.centerPos = boss_->GetPos();
	bossCollider.radius = 1.f;

	bool isHit = Collision::SphereHitSphere(playerFront, bossCollider);

	if (isHit == true)
	{
		// y軸を無視する
		Vec3 pos1 = player_->GetPos() * Vec3(1, 0, 1);
		Vec3 pos2 = boss_->GetPos() * Vec3(1, 0, 1);

		float radius =
			player_->GetHumanoidBody()->GetBodyCollider().radius +
			boss_->GetCollider().radius;
		*dis = Vec3::Distance(pos1, pos2) - radius;
	}

	return isHit;
}

void CollisionManager::SetPlayer(Player* player)
{
	player_ = player;
}

void CollisionManager::SetBoss(Boss* boss)
{
	boss_ = boss;
}
