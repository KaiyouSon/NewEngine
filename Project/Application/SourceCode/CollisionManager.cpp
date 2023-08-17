#include "CollisionManager.h"
#include "EffectManager.h"

void CollisionManager::Update()
{
	// プレイヤーとボス
	PlayerHitBoss();

	// プレイヤーとメッセージサイン
	//PlayerHitMessageSign();

	// プレイヤーとリスポーン地点
	//PlayerHitRespawnPoint();

	PlayerHitFieldObject();

	PlayerHitNegotiation();

	// ボスとプレイヤー
	//BossHitPlayer();
}

void CollisionManager::PushBackPlayer()
{
}

void CollisionManager::PlayerHitBoss()
{
	static Vec3 hitPoint = {};

	bool isAttackBoss =
		Collision::CapsuleHitCapsule(
			mPlayer->GetWeapon()->collider,
			mBoss->GetCollider(),
			hitPoint);

	if (isAttackBoss)
	{
		if (mBoss->GetisAlive() == true)
		{
			if (mBoss->GetisDamage() == false)
			{
				mBoss->Damage(mPlayer->GetWeapon()->GetDamage());

				SoundManager::Play("HitSE");
				EffectManager::GetInstance()->GenerateBloodSprayEffect(hitPoint);

				mBoss->SetisDamage(true);
			}
		}
	}

	bool isBodyTouch =
		Collision::CapsuleHitCapsule(
			mPlayer->GetBodyCollider(),
			mBoss->GetCollider());

	if (isBodyTouch == true)
	{
		// 半径を足して実際の長さを求める
		float checkLength =
			mPlayer->GetBodyCollider().radius +
			mBoss->GetCollider().radius;

		// y軸を無視する
		Vec3 pos1 = mPlayer->GetPos() * Vec3(1, 0, 1);
		Vec3 pos2 = mBoss->GetPos() * Vec3(1, 0, 1);

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

		Vec3 nextPos = mPlayer->GetPos() + pushVec;
		mPlayer->SetPos(nextPos);
	}
}
void CollisionManager::PlayerHitMessageSign()
{
	for (const auto& messageSign : *mField->GetMessageSigns())
	{
		if (Collision::SphereHitCapsule(
			messageSign->GetCollider(), mPlayer->GetBodyCollider()))
		{
			mUiManager->GetNegotiationUI()->SetisActive(true);
			mUiManager->GetNegotiationUI()->SetStrType(NegotiationUI::ReadMessageStr);
			mUiManager->GetMessageUI()->SetTexture(messageSign->GetMessageTexture());
			break;
		}
		else
		{
			mUiManager->GetNegotiationUI()->SetisActive(false);
		}
	}
}
void CollisionManager::PlayerHitRespawnPoint()
{
	//if (Collision::SphereHitCapsule(
	//	mField->GetRespawnPoint()->GetCollider(), mPlayer->GetBodyCollider()))
	//{
	//	mUiManager->GetNegotiationUI()->SetisActive(true);
	//	mUiManager->GetNegotiationUI()->SetStrType(NegotiationUI::RestInLightStr);
	//}
	//else
	//{
	//	mUiManager->GetNegotiationUI()->SetisActive(false);
	//}
}

void CollisionManager::PlayerHitNegotiation()
{
	// 何かに当たった時用
	bool isHit = false;

	// リスポーン地点と当たったら
	for (const auto& respawnPoint : mField->GetFieldData()->respawnPoints)
	{
		if (Collision::SphereHitCapsule(
			respawnPoint->GetCollider(), mPlayer->GetBodyCollider()))
		{
			mUiManager->GetNegotiationUI()->SetisActive(true);
			mUiManager->GetNegotiationUI()->SetStrType(NegotiationUI::RestInLightStr);
			isHit = true;
			return;
		}
	}

	// メッセージサインと当たったら
	for (const auto& messageSign : *mField->GetMessageSigns())
	{
		if (Collision::SphereHitCapsule(
			messageSign->GetCollider(), mPlayer->GetBodyCollider()))
		{
			mUiManager->GetNegotiationUI()->SetisActive(true);
			mUiManager->GetNegotiationUI()->SetStrType(NegotiationUI::ReadMessageStr);
			mUiManager->GetMessageUI()->SetTexture(messageSign->GetMessageTexture());
			isHit = true;
			return;
		}
	}

	// 何も当たってなかった時
	if (isHit == false)
	{
		mUiManager->GetNegotiationUI()->SetisActive(false);
	}



	//// リスポーン地点と当たったら
	//if (Collision::SphereHitCapsule(
	//	mField->GetRespawnPoint()->GetCollider(), mPlayer->GetBodyCollider()))
	//{
	//	mUiManager->GetNegotiationUI()->SetisActive(true);
	//	mUiManager->GetNegotiationUI()->SetStrType(NegotiationUI::RestInLightStr);
	//	return;
	//}
	//else
	//{
	//	// メッセージサインと当たったら
	//	for (const auto& messageSign : *mField->GetMessageSigns())
	//	{
	//		if (Collision::SphereHitCapsule(
	//			messageSign->GetCollider(), mPlayer->GetBodyCollider()))
	//		{
	//			mUiManager->GetNegotiationUI()->SetisActive(true);
	//			mUiManager->GetNegotiationUI()->SetStrType(NegotiationUI::ReadMessageStr);
	//			mUiManager->GetMessageUI()->SetTexture(messageSign->GetMessageTexture());
	//			break;
	//		}
	//		else
	//		{

	//		}
	//	}
	//}

}
void CollisionManager::PlayerHitFieldObject()
{
	FieldData* fieldData = mField->GetFieldData();

	const std::vector<std::unique_ptr<Coffin>>& coffins = fieldData->coffins;
	for (uint32_t i = 0; i < coffins.size(); i++)
	{
		Vec3 hitPoint = 0;

		if (Collision::CubeHitCapsule(
			coffins[i]->GetBottomCollider(), mPlayer->GetBodyCollider(), hitPoint))
		{
			mField->SetSpherePos(hitPoint);

			// y軸を無視する
			Vec3 pos1 = mPlayer->GetPos() * Vec3(1, 0, 1);
			Vec3 pos2 = hitPoint * Vec3(1, 0, 1);

			// プレイヤーに向かうベクトル
			Vec3 toPlayer = pos1 - pos2;
			float toPlayerLength = toPlayer.Length();
			Vec3 normal = toPlayer.Norm();

			// 衝突した位置とプレイヤーの中心が重なる距離を計算
			float overlap = mPlayer->GetBodyCollider().radius - toPlayerLength;

			// 押し戻しのベクトル
			Vec3 pushVec = normal * (overlap);

			Vec3 nextPos = mPlayer->GetPos() + pushVec;
			mPlayer->SetPos(nextPos);
		}
	}
}

void CollisionManager::BossHitPlayer()
{
	if (mPlayer->GetisAlive() == false ||
		mBoss->GetisAlive() == false)
	{
		return;
	}

	static Vec3 hitPoint = {};

	bool isAttackBoss =
		Collision::CapsuleHitCapsule(
			mBoss->GetWeapon()->collider,
			mPlayer->GetBodyCollider(),
			hitPoint);

	if (isAttackBoss)
	{
		if (mPlayer->GetState() != Player::State::Roll &&
			mPlayer->GetState() != Player::State::Backstep)
		{
			if (mPlayer->GetisDamage() == false)
			{
				SoundManager::Play("HitSE");
				mPlayer->Damage(mBoss->GetDamage() * 1.5f);
				EffectManager::GetInstance()->GenerateBloodSprayEffect(hitPoint);

				mPlayer->SetisDamage(true);
			}
		}
	}
}

bool CollisionManager::IsCheckFrontBoss(const Vec3 pos, const Vec3 front)
{
	SphereCollider frontCollider;
	frontCollider.centerPos = pos + front;
	frontCollider.radius = 1.0f;

	SphereCollider bossCollider;
	bossCollider.centerPos = mBoss->GetPos();
	bossCollider.radius = 1.0f;

	bool isHit = Collision::SphereHitSphere(frontCollider, bossCollider);
	return isHit;
}
float CollisionManager::CalcPlayerDisToFront(const Vec3 frontVec, const float max)
{
	RayCollider playerFront;
	playerFront.startPos = mPlayer->GetPos();
	playerFront.dirVec = frontVec;

	SphereCollider bossCollider;
	bossCollider.centerPos = mBoss->GetPos();
	bossCollider.radius = 1.f;

	bool isHit = Collision::RayHitSphere(playerFront, bossCollider);

	float dis = max;
	if (isHit == true)
	{
		// y軸を無視する
		Vec3 pos1 = mPlayer->GetPos() * Vec3(1, 0, 1);
		Vec3 pos2 = mBoss->GetPos() * Vec3(1, 0, 1);

		float radius =
			mPlayer->GetBodyCollider().radius +
			mBoss->GetCollider().radius;
		dis = Vec3::Distance(pos1, pos2) - radius;
		dis = Min<float>(dis, max);
	}

	return dis;
}

void CollisionManager::SetPlayer(Player* player)
{
	mPlayer = player;
}
void CollisionManager::SetBoss(Boss* boss)
{
	mBoss = boss;
}
void CollisionManager::SetField(Field* field)
{
	mField = field;
}
void CollisionManager::SetUIManager(UIManager* uiManager)
{
	mUiManager = uiManager;
}
