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
		// ”¼Œa‚ğ‘«‚µ‚ÄÀÛ‚Ì’·‚³‚ğ‹‚ß‚é
		float checkLength =
			player_->GetHumanoidBody()->GetBodyCollider().radius +
			boss_->GetCollider().radius;

		// y²‚ğ–³‹‚·‚é
		Vec3 pos1 = player_->GetPos() * Vec3(1, 0, 1);
		Vec3 pos2 = boss_->GetPos() * Vec3(1, 0, 1);

		Vec3 toPlayer = pos1 - pos2;

		// Œ»İ‚Ì’·‚³
		float curLength = toPlayer.Length();

		// checkLength ‚Æ curLength ‚Ì·•ª‚Å–„‚ß‚ñ‚¾’·‚³‚ª‹‚Ü‚é
		float embedLength = checkLength - curLength;

		// Œë·‚ğ‚¯‚·ˆ—
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

float CollisionManager::CalcPlayerDisToFront(const Vec3 frontVec, const float max)
{
	RayCollider playerFront;
	playerFront.startPos = player_->GetPos();
	playerFront.dirVec = frontVec;

	SphereCollider bossCollider;
	bossCollider.centerPos = boss_->GetPos();
	bossCollider.radius = 1.f;

	bool isHit = Collision::RayHitSphere(playerFront, bossCollider);

	float dis = max;
	if (isHit == true)
	{
		// y²‚ğ–³‹‚·‚é
		Vec3 pos1 = player_->GetPos() * Vec3(1, 0, 1);
		Vec3 pos2 = boss_->GetPos() * Vec3(1, 0, 1);

		float radius =
			player_->GetHumanoidBody()->GetBodyCollider().radius +
			boss_->GetCollider().radius;
		dis = Vec3::Distance(pos1, pos2) - radius;
		dis = Min<float>(dis, max);
	}

	return dis;
}

//bool CollisionManager::IsCheckPlayerMove(const Vec3 frontVec, float* dis)
//{
//	RayCollider playerFront;
//	playerFront.startPos = player_->GetPos();
//	playerFront.dirVec = frontVec;
//
//	SphereCollider bossCollider;
//	bossCollider.centerPos = boss_->GetPos();
//	bossCollider.radius = 1.f;
//
//	bool isHit = Collision::RayHitSphere(playerFront, bossCollider);
//
//	if (isHit == true)
//	{
//		// y²‚ğ–³‹‚·‚é
//		Vec3 pos1 = player_->GetPos() * Vec3(1, 0, 1);
//		Vec3 pos2 = boss_->GetPos() * Vec3(1, 0, 1);
//
//		float radius =
//			player_->GetHumanoidBody()->GetBodyCollider().radius +
//			boss_->GetCollider().radius;
//		*dis = Vec3::Distance(pos1, pos2) - radius;
//	}
//
//	return isHit;
//}

void CollisionManager::SetPlayer(Player* player)
{
	player_ = player;
}

void CollisionManager::SetBoss(Boss* boss)
{
	boss_ = boss;
}
