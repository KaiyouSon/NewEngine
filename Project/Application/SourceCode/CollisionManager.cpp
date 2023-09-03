#include "CollisionManager.h"
#include "EffectManager.h"
#include "TransitionManager.h"

void CollisionManager::Update()
{
	// �v���C���[�ƃ{�X
	PlayerHitBoss();

	PlayerHitFieldObject();

	PlayerHitNegotiation();

	// �{�X�ƃv���C���[
	BossHitPlayer();
}

void CollisionManager::PushBackPlayer()
{
}

void CollisionManager::PlayerHitBoss()
{
	static Vec3 hitPoint = {};

	float dis = Vec3::Distance(mPlayer->GetPos(), mBoss->GetPos());
	if (dis >= 50)
	{
		return;
	}

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
		// ���a�𑫂��Ď��ۂ̒��������߂�
		float checkLength =
			mPlayer->GetBodyCollider().radius +
			mBoss->GetCollider().radius;

		// y���𖳎�����
		Vec3 pos1 = mPlayer->GetPos() * Vec3(1, 0, 1);
		Vec3 pos2 = mBoss->GetPos() * Vec3(1, 0, 1);

		Vec3 toPlayer = pos1 - pos2;

		// ���݂̒���
		float curLength = toPlayer.Length();

		// checkLength �� curLength �̍����Ŗ��ߍ��񂾒��������܂�
		float embedLength = checkLength - curLength;

		// �덷����������
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
void CollisionManager::PlayerHitNegotiation()
{
	// �����ɓ����������p
	bool isHit = false;

	// ���X�|�[���n�_�Ɠ���������
	for (const auto& respawnPoint : mField->GetFieldData()->respawnPoints)
	{
		float dis = Vec3::Distance(mPlayer->GetPos(), respawnPoint->GetPos());
		if (dis >= 20)
		{
			continue;;
		}

		if (Collision::SphereHitCapsule(
			respawnPoint->GetCollider(), mPlayer->GetBodyCollider()))
		{
			mUiManager->GetNegotiationUI()->SetisActive(true);
			mUiManager->GetNegotiationUI()->SetStrType(NegotiationUI::RestInLightStr);
			isHit = true;

			if (Pad::GetButtonDown(PadCode::ButtonB))
			{
				TransitionManager::GetInstance()->Start(TransitionType::Respawn);
			}
			return;
		}
	}

	// ���b�Z�[�W�T�C���Ɠ���������
	for (const auto& messageSign : *mField->GetMessageSigns())
	{
		float dis = Vec3::Distance(mPlayer->GetPos(), messageSign->GetPos());
		if (dis >= 20)
		{
			continue;
		}

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

	// �����������ĂȂ�������
	if (isHit == false)
	{
		mUiManager->GetNegotiationUI()->SetisActive(false);
	}
}
void CollisionManager::PlayerHitFieldObject()
{
	FieldData* fieldData = mField->GetFieldData();

	// ����
	const std::vector<std::unique_ptr<Coffin>>& coffins = fieldData->coffins;
	for (uint32_t i = 0; i < coffins.size(); i++)
	{
		float dis = Vec3::Distance(mPlayer->GetPos(), coffins[i]->GetPos());
		if (dis >= 20)
		{
			continue;
		}

		Vec3 hitPoint = 0;
		if (Collision::CubeHitCapsule(
			coffins[i]->GetBottomCollider(), mPlayer->GetBodyCollider(), hitPoint))
		{
			mField->SetSpherePos(hitPoint);

			// y���𖳎�����
			Vec3 pos1 = mPlayer->GetPos() * Vec3(1, 0, 1);
			Vec3 pos2 = hitPoint * Vec3(1, 0, 1);

			// �v���C���[�Ɍ������x�N�g��
			Vec3 toPlayer = pos1 - pos2;
			float toPlayerLength = toPlayer.Length();
			Vec3 normal = toPlayer.Norm();

			// �Փ˂����ʒu�ƃv���C���[�̒��S���d�Ȃ鋗�����v�Z
			float overlap = mPlayer->GetBodyCollider().radius - toPlayerLength;

			// �����߂��̃x�N�g��
			Vec3 pushVec = normal * (overlap);

			Vec3 nextPos = mPlayer->GetPos() + pushVec;
			mPlayer->SetPos(nextPos);
		}
	}

	// ��
	const std::vector<std::unique_ptr<Tree>>& trees = fieldData->trees;
	for (uint32_t i = 0; i < trees.size(); i++)
	{
		float dis = Vec3::Distance(mPlayer->GetPos(), trees[i]->GetPos());
		if (dis >= 20)
		{
			continue;
		}

		Vec3 hitPoint = 0;
		if (Collision::SphereHitCapsule(
			trees[i]->GetCollider(), mPlayer->GetBodyCollider(), hitPoint))
		{
			mField->SetSpherePos(hitPoint);

			// ���a�𑫂��Ď��ۂ̒��������߂�
			float checkLength =
				mPlayer->GetBodyCollider().radius + trees[i]->GetCollider().radius;

			// y���𖳎�����
			Vec3 pos1 = mPlayer->GetPos() * Vec3(1, 0, 1);
			Vec3 pos2 = trees[i]->GetPos() * Vec3(1, 0, 1);

			Vec3 toPlayer = pos1 - pos2;

			// ���݂̒���
			float curLength = toPlayer.Length();

			// checkLength �� curLength �̍����Ŗ��ߍ��񂾒��������܂�
			float embedLength = checkLength - curLength;

			// �덷����������
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

	// ��
	const std::vector<std::unique_ptr<Wall>>& walls = fieldData->walls;
	for (uint32_t i = 0; i < walls.size(); i++)
	{
		float dis = Vec3::Distance(mPlayer->GetPos(), walls[i]->GetPos());
		if (dis >= 50)
		{
			continue;
		}

		Vec3 hitPoint = 0;
		if (Collision::CapsuleHitCapsule(
			walls[i]->GetCollider(), mPlayer->GetBodyCollider(), hitPoint))
		{
			mField->SetSpherePos(hitPoint);

			// y���𖳎�����
			Vec3 pos1 = mPlayer->GetPos() * Vec3(1, 0, 1);
			Vec3 pos2 = hitPoint * Vec3(1, 0, 1);

			// �v���C���[�Ɍ������x�N�g��
			Vec3 toPlayer = pos1 - pos2;
			float toPlayerLength = toPlayer.Length();
			Vec3 normal = toPlayer.Norm();

			// �Փ˂����ʒu�ƃv���C���[�̒��S���d�Ȃ鋗�����v�Z
			float overlap = mPlayer->GetBodyCollider().radius - toPlayerLength;

			// �����߂��̃x�N�g��
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
		// y���𖳎�����
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
