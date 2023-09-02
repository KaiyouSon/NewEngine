#include "ColliderDrawer.h"
#include "ModelManager.h"
#include "Collider.h"

uint32_t ColliderDrawer::index = 0;

void ColliderDrawer::Load()
{
	// �R���C�_�[���f���̃��[�h
	ModelManager::LoadObjModel("Collider/CircleCollider", "CircleCollider");
	ModelManager::LoadObjModel("Collider/SquareCollider", "SquareCollider");
	ModelManager::LoadObjModel("Collider/SphereCollider", "SphereCollider");
	ModelManager::LoadObjModel("Collider/CubeCollider", "CubeCollider");
	ModelManager::LoadObjModel("Collider/CapsuleColliderParts1", "CapsuleColliderParts1");
	ModelManager::LoadObjModel("Collider/CapsuleColliderParts2", "CapsuleColliderParts2");

	// �~
	mModels.insert(std::make_pair("CircleCollider", ModelManager::GetModel("CircleCollider")));

	// ��`
	mModels.insert(std::make_pair("SquareCollider", ModelManager::GetModel("SquareCollider")));

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

	mIsShow = false;
}

void ColliderDrawer::Update()
{
	if (mIsShow == false)
	{
		return;
	}

	for (const auto& colliderObjcet : mColliderObjects)
	{
		colliderObjcet->Update();
	}
}

void ColliderDrawer::DrawCollider()
{
	if (mIsShow == false)
	{
		index = 0;
		return;
	}

	for (const auto& colliderObjcet : mColliderObjects)
	{
		colliderObjcet->Draw();
	}

	// ���̃t���[���̏���
	index = 0;
}

void ColliderDrawer::SetisShow(const bool isShow)
{
	mIsShow = isShow;
}

bool ColliderDrawer::GetisShow()
{
	return mIsShow;
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

	if (collider->isActive == false)
	{
		OutputDebugLog("Collider->isActive��false�ł�");
		return;
	}

	// �R���C�_�[���\�����邽�߂ɕK�v�Ȑ����i�[����
	switch (collider->primitive)
	{
	case ColliderPrimitive::Circle:
	{
		// �L���X�g
		CircleCollider* castCollider = dynamic_cast<CircleCollider*>(collider);
		mColliderObjects[index]->model = mModels["CircleCollider"];
		mColliderObjects[index]->transform.pos = castCollider->centerPos;
		mColliderObjects[index]->transform.scale = castCollider->radius;
		mColliderObjects[index]->transform.rot = 0;
		mColliderObjects[index]->color = Color::white;
		mColliderObjects[index]->is3D = false;
		index++;
	}
	break;

	case ColliderPrimitive::Square:
	{
		// �L���X�g
		SquareCollider* castCollider = dynamic_cast<SquareCollider*>(collider);
		mColliderObjects[index]->model = mModels["SquareCollider"];
		mColliderObjects[index]->transform.pos = castCollider->centerPos;
		mColliderObjects[index]->transform.scale = castCollider->size;
		mColliderObjects[index]->transform.rot = 0;
		mColliderObjects[index]->color = Color::white;
		mColliderObjects[index]->is3D = false;
		index++;
	}
	break;

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

		Vec3 v = castCollider->endPos - castCollider->startPos;
		Vec3 rot =
		{
			atan2f(-v.z,-v.y),
			atan2f(-v.z,v.x),
			atan2f(v.x,-v.y),
		};

		// ���̔���
		mColliderObjects[index]->model = mModels["CapsuleColliderParts1"];
		mColliderObjects[index]->transform.pos = castCollider->startPos;
		mColliderObjects[index]->transform.scale = castCollider->radius;
		mColliderObjects[index]->transform.rot = rot + Vec3(0, 0, Radian(180));
		mColliderObjects[index]->color = col;
		index++;

		// �㔼��
		mColliderObjects[index]->model = mModels["CapsuleColliderParts1"];
		mColliderObjects[index]->transform.pos = castCollider->endPos;
		mColliderObjects[index]->transform.scale = castCollider->radius;
		mColliderObjects[index]->transform.rot = rot;
		mColliderObjects[index]->color = Color::white;
		index++;

		// �~��
		Vec3 halfPos = v / 2;
		float length = v.Length() / 2;

		mColliderObjects[index]->model = mModels["CapsuleColliderParts2"];
		mColliderObjects[index]->transform.pos = castCollider->startPos + halfPos;
		mColliderObjects[index]->transform.scale = Vec3(castCollider->radius, length, castCollider->radius);
		mColliderObjects[index]->transform.rot = rot;
		mColliderObjects[index]->color = col;
		index++;
	}
	break;

	}

}
