#include "ColliderDrawer.h"
#include "ModelManager.h"
#include "Collider.h"

uint32_t ColliderDrawer::index = 0;

void ColliderDrawer::Load()
{
	// コライダーモデルのロード
	ModelManager::LoadObjModel("Collider/CircleCollider", "CircleCollider");
	ModelManager::LoadObjModel("Collider/SquareCollider", "SquareCollider");
	ModelManager::LoadObjModel("Collider/SphereCollider", "SphereCollider");
	ModelManager::LoadObjModel("Collider/CubeCollider", "CubeCollider");
	ModelManager::LoadObjModel("Collider/CapsuleColliderParts1", "CapsuleColliderParts1");
	ModelManager::LoadObjModel("Collider/CapsuleColliderParts2", "CapsuleColliderParts2");

	// 円
	mModels.insert(std::make_pair("CircleCollider", ModelManager::GetModel("CircleCollider")));

	// 矩形
	mModels.insert(std::make_pair("SquareCollider", ModelManager::GetModel("SquareCollider")));

	// 球体
	mModels.insert(std::make_pair("SphereCollider", ModelManager::GetModel("SphereCollider")));

	// キューブ
	mModels.insert(std::make_pair("CubeCollider", ModelManager::GetModel("CubeCollider")));

	// 半球体
	mModels.insert(std::make_pair("CapsuleColliderParts1", ModelManager::GetModel("CapsuleColliderParts1")));

	// 円柱(上下に面がない)
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

	// 次のフレームの準備
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
		OutputDebugLog("MaxColliderNumの数が足りません");
		return;
	}

	if (collider == nullptr)
	{
		OutputDebugLog("BindしているColliderがnullptrです");
		return;
	}

	if (collider->isActive == false)
	{
		OutputDebugLog("Collider->isActiveがfalseです");
		return;
	}

	// コライダーを構成するために必要な数分格納する
	switch (collider->primitive)
	{
	case ColliderPrimitive::Circle:
	{
		// キャスト
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
		// キャスト
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
		// キャスト
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
		// キャスト
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
		// キャスト
		CapsuleCollider* castCollider = dynamic_cast<CapsuleCollider*>(collider);

		Color col = Color::white;

		Vec3 v = castCollider->endPos - castCollider->startPos;
		Vec3 rot =
		{
			atan2f(-v.z,-v.y),
			atan2f(-v.z,v.x),
			atan2f(v.x,-v.y),
		};

		// 下の半球
		mColliderObjects[index]->model = mModels["CapsuleColliderParts1"];
		mColliderObjects[index]->transform.pos = castCollider->startPos;
		mColliderObjects[index]->transform.scale = castCollider->radius;
		mColliderObjects[index]->transform.rot = rot + Vec3(0, 0, Radian(180));
		mColliderObjects[index]->color = col;
		index++;

		// 上半球
		mColliderObjects[index]->model = mModels["CapsuleColliderParts1"];
		mColliderObjects[index]->transform.pos = castCollider->endPos;
		mColliderObjects[index]->transform.scale = castCollider->radius;
		mColliderObjects[index]->transform.rot = rot;
		mColliderObjects[index]->color = Color::white;
		index++;

		// 円柱
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
