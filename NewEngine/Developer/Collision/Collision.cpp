#include "Collision.h"
using namespace std;

//bool Collision::OnTrigger(const GameObject* obj1, const GameObject* obj2)
//{
//	for (const auto& collider1 : obj1->colliderManager->list)
//	{
//		for (const auto& collider2 : obj2->colliderManager->list)
//		{
//			if (collider1->GetColliderType() == SphereShape &&
//				collider2->GetColliderType() == SphereShape)
//			{
//				if (SphereHitSphere(
//					*collider1->GetSphereCollider(),
//					*collider2->GetSphereCollider()) == true)
//				{
//					return true;
//				}
//				else
//				{
//					continue;
//				}
//			}
//
//			// レイとメッシュ
//			if (collider1->GetColliderType() == ReyShape &&
//				collider2->GetColliderType() == MeshShape)
//			{
//				if (ReyHitMesh(
//					*collider1->GetReyCollider(),
//					*collider2->GetMeshCollider()))
//				{
//					return true;
//				}
//				else
//				{
//					continue;
//				}
//			}
//
//			if (collider1->GetColliderType() == MeshShape &&
//				collider2->GetColliderType() == ReyShape)
//			{
//				if (ReyHitMesh(
//					*collider2->GetReyCollider(),
//					*collider1->GetMeshCollider()))
//				{
//					return true;
//				}
//				else
//				{
//					continue;
//				}
//			}
//
//			// 線とメッシュ
//			if (collider1->GetColliderType() == LineShape &&
//				collider2->GetColliderType() == MeshShape)
//			{
//				if (LineHitMesh(
//					*collider1->GetLineCollider(),
//					*collider2->GetMeshCollider()))
//				{
//					return true;
//				}
//				else
//				{
//					continue;
//				}
//			}
//
//			if (collider1->GetColliderType() == MeshShape &&
//				collider2->GetColliderType() == LineShape)
//			{
//				if (LineHitMesh(
//					*collider2->GetLineCollider(),
//					*collider1->GetMeshCollider()))
//				{
//					return true;
//				}
//				else
//				{
//					continue;
//				}
//			}
//		}
//	}
//
//	return false;
//}

bool Collision::SphereHitSphere(const SphereCollider& sphere1, const SphereCollider& sphere2)
{
	return
	{
		(sphere2.centerPos.x - sphere1.centerPos.x) *
		(sphere2.centerPos.x - sphere1.centerPos.x) +
		(sphere2.centerPos.y - sphere1.centerPos.y) *
		(sphere2.centerPos.y - sphere1.centerPos.y) +
		(sphere2.centerPos.z - sphere1.centerPos.z) *
		(sphere2.centerPos.z - sphere1.centerPos.z) <=
		(sphere2.radius + sphere1.radius) *
		(sphere2.radius + sphere1.radius)
	};
}

bool Collision::ReyHitMesh(const ReyCollider& rey, const MeshCollider& mesh)
{
	// レイの始点と原点の距離
	float dis1 = Vec3::Distance(rey.startPos, Vec3::zero);
	// メッシュの中心座標と原点の距離
	float dis2 = Vec3::Distance(mesh.centerPos, Vec3::zero);
	// レイの始点とメッシュ距離
	float dist = dis1 - dis2;
	// メッシュの法線とレイのベクトルでcosθを求める
	float cosRadius = Vec3::Dot(mesh.normal * -1, rey.dirVec.Normalized());

	// 当たった点
	Vec3 inter = rey.startPos + rey.dirVec.Normalized() * dist;

	if (inter.x >= mesh.lowerLeftPos.x && inter.x <= mesh.lowerRightPos.x &&
		inter.y >= mesh.lowerLeftPos.y && inter.y <= mesh.upperLeftPos.y)
		return true;

	return false;
}

bool Collision::LineHitMesh(const LineCollider& line, const MeshCollider& mesh)
{
	Vec3 v1 = line.startPos - mesh.centerPos;
	Vec3 v2 = line.endPos - mesh.centerPos;
	//Vec3 v3 = v1 + v2;

	if (Vec3::Dot(v1, mesh.normal * Vec3::Dot(v2, mesh.normal)) <= 0)
	{
		if (v1.x >= mesh.lowerLeftPos.x && v1.x <= mesh.lowerRightPos.x &&
			v1.y >= mesh.lowerLeftPos.y && v1.y <= mesh.upperLeftPos.y)
		{
			return true;
		}
		if (v2.x >= mesh.lowerLeftPos.x && v2.x <= mesh.lowerRightPos.x &&
			v2.y >= mesh.lowerLeftPos.y && v2.y <= mesh.upperLeftPos.y)
		{
			return true;
		}

	}
	return false;
}