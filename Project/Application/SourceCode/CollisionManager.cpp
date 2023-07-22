#include "CollisionManager.h"

void CollisionManager::Update()
{
	// プレイヤーとボス
	PlayerHitBoss();

	// プレイヤーとメッセージサイン
	PlayerHitMessageSign();
}

void CollisionManager::PlayerHitBoss()
{
	bool isAttackBoss =
		Collision::CapsuleHitCapsule(
			player_->GetWeapon()->collider,
			boss_->GetCollider());

	if (isAttackBoss)
	{
		boss_->Damage(player_->GetWeapon()->GetDamage());
	}

	bool isBodyTouch =
		Collision::CapsuleHitCapsule(
			player_->GetBodyCollider(),
			boss_->GetCollider());

	if (isBodyTouch == true)
	{
		// 半径を足して実際の長さを求める
		float checkLength =
			player_->GetBodyCollider().radius +
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
void CollisionManager::PlayerHitMessageSign()
{
	MessageSign* cur = field_->GetMessageSign();

	if (Collision::SphereHitCapsule(
		cur->GetCollider(), player_->GetBodyCollider()))
	{
		uiManager_->GetNegotiationUI()->SetisActive(true);
	}
	else
	{
		uiManager_->GetNegotiationUI()->SetisActive(false);
	}
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
		// y軸を無視する
		Vec3 pos1 = player_->GetPos() * Vec3(1, 0, 1);
		Vec3 pos2 = boss_->GetPos() * Vec3(1, 0, 1);

		float radius =
			player_->GetBodyCollider().radius +
			boss_->GetCollider().radius;
		dis = Vec3::Distance(pos1, pos2) - radius;
		dis = Min<float>(dis, max);
	}

	return dis;
}

void CollisionManager::SetPlayer(Player* player)
{
	player_ = player;
}
void CollisionManager::SetBoss(Boss* boss)
{
	boss_ = boss;
}
void CollisionManager::SetField(Field* field)
{
	field_ = field;
}
void CollisionManager::SetUIManager(UIManager* uiManager)
{
	uiManager_ = uiManager;
}
