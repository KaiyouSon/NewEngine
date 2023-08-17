#include "ColliderDrawer.h"
#include "ModelManager.h"
#include "Collider.h"

uint32_t ColliderDrawer::index = 0;

void ColliderDrawer::Load()
{
	// �R���C�_�[���f���̃��[�h
	ModelManager::LoadObjModel("Collider/SphereCollider", "SphereCollider");
	ModelManager::LoadObjModel("Collider/CubeCollider", "CubeCollider");
	ModelManager::LoadObjModel("Collider/CapsuleColliderParts1", "CapsuleColliderParts1");
	ModelManager::LoadObjModel("Collider/CapsuleColliderParts2", "CapsuleColliderParts2");

	// ����
	mModels.insert(std::make_pair("SphereCollider", ModelManager::GetModel("SphereCollider")));

	// �L���[�u
	mModels.insert(std::make_pair("CubeCollider", ModelManager::GetModel("CubeCollider")));

	// ������
	mModels.insert(std::make_pair("CapsuleColliderParts1", ModelManager::GetModel("CapsuleColliderParts1")));

	// �~��(�㉺�ɖʂ��Ȃ�)
	mModels.insert(std::make_pair("CapsuleColliderParts2", ModelManager::GetModel("CapsuleColliderParts2")));


}

void ColliderDrawer::Init()
{
	mMaxColliderNum = 64;

	for (uint32_t i = 0; i < mMaxColliderNum; i++)
	{
		mColliderObjects.push_back(std::make_unique<ColliderObject>());
	}
}

void ColliderDrawer::Update()
{
	for (const auto& colliderObjcet : mColliderObjects)
	{
		colliderObjcet->Update();
	}
}

void ColliderDrawer::DrawCollider()
{
	for (const auto& colliderObjcet : mColliderObjects)
	{
		colliderObjcet->Draw();
	}

	// ���̃t���[���̏���
	index = 0;
}

void ColliderDrawer::Bind(ICollider* collider)
{
	if (index >= mMaxColliderNum)
	{
		OutputDebugLog("MaxColliderNum�̐�������܂���");
		return;
	}

	if (collider == nullptr)
	{
		OutputDebugLog("Bind���Ă���Collider��nullptr�ł�");

		return;
	}

	// �R���C�_�[���\�����邽�߂ɕK�v�Ȑ����i�[����
	switch (collider->primitive)
	{
	case ColliderPrimitive::Sphere:
	{
		// �L���X�g
		SphereCollider* castCollider = dynamic_cast<SphereCollider*>(collider);
		mColliderObjects[index]->model = mModels["SphereCollider"];
		mColliderObjects[index]->transform.pos = castCollider->centerPos;
		mColliderObjects[index]->transform.scale = castCollider->radius;
		mColliderObjects[index]->transform.rot = 0;
		mColliderObjects[index]->color = Color::white;
		index++;
	}
	break;

	case ColliderPrimitive::Cube:
	{
		// �L���X�g
		CubeCollider* castCollider = dynamic_cast<CubeCollider*>(collider);
		mColliderObjects[index]->model = mModels["CubeCollider"];
		mColliderObjects[index]->transform.pos = castCollider->centerPos;
		mColliderObjects[index]->transform.scale = castCollider->size;
		mColliderObjects[index]->transform.rot = 0;
		mColliderObjects[index]->color = Color::white;
		index++;
	}
	break;


	case ColliderPrimitive::Capsule:
	{
		// �L���X�g
		CapsuleCollider* castCollider = dynamic_cast<CapsuleCollider*>(collider);

		Color col = Color::white;

		// ���̔���
		mColliderObjects[index]->model = mModels["CapsuleColliderParts1"];
		mColliderObjects[index]->transform.pos = castCollider->startPos;
		mColliderObjects[index]->transform.scale = castCollider->radius;
		mColliderObjects[index]->transform.rot.z = Radian(180);
		mColliderObjects[index]->color = col;
		index++;

		// �㔼��
		mColliderObjects[index]->model = mModels["CapsuleColliderParts1"];
		mColliderObjects[index]->transform.pos = castCollider->endPos;
		mColliderObjects[index]->transform.scale = castCollider->radius;
		mColliderObjects[index]->transform.rot = 0;
		mColliderObjects[index]->color = Color::white;
		index++;

		// �~��
		Vec3 v = castCollider->endPos - castCollider->startPos;
		Vec3 halfPos = v / 2;
		float length = v.Length() / 2;

		mColliderObjects[index]->model = mModels["CapsuleColliderParts2"];
		mColliderObjects[index]->transform.pos = castCollider->startPos + halfPos;
		mColliderObjects[index]->transform.scale = Vec3(castCollider->radius, length, castCollider->radius);
		mColliderObjects[index]->transform.rot = 0;
		mColliderObjects[index]->color = col;
		index++;
	}
	break;

	}

}
