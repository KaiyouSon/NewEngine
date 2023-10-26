#pragma once
#include "Collider.h"

// 当たり判定の関数
namespace Collision
{
	// 円と円の当たり判定
	bool CircleHitCircle(
		const CircleCollider& circle1,
		const CircleCollider& circle2);

	// 球と球の当たり判定
	bool SphereHitSphere(
		const SphereCollider& sphere1,
		const SphereCollider& sphere2);

	// 立方体と立方体の当たり判定
	bool CubeHitCube(
		const CubeCollider& cube1,
		const CubeCollider& cube2);

	// 立方体とカプセルの当たり判定
	bool CubeHitCapsule(
		CubeCollider& cube,
		CapsuleCollider& capsule);

	bool CubeHitCapsule(
		const CubeCollider& cube,
		const CapsuleCollider& capsule,
		Vec3& hitPoint);

	// 球と平面の当たり判定
	bool SphereHitPlane(
		const SphereCollider& sphere,
		const PlaneCollider& plane,
		Vec3* hitPoint = nullptr);

	// 球と三角形の当たり判定
	bool SphereHitTriangle(
		const SphereCollider& sphere,
		const TriangleCollider& triangle,
		Vec3* hitPoint = nullptr);

	// 球とカプセルの当たり判定
	bool SphereHitCapsule(
		const SphereCollider& sphere,
		const CapsuleCollider& capsule);

	bool SphereHitCapsule(
		const SphereCollider& sphere,
		const CapsuleCollider& capsule,
		Vec3& hitPoint);

	// レイと平面の当たり判定
	bool RayHitPlane(
		const RayCollider& ray,
		const PlaneCollider& plane);

	// レイと球の当たり判定
	bool RayHitSphere(
		const RayCollider& ray,
		const SphereCollider& sphere);

	// カプセルとカプセルの当たり判定
	bool CapsuleHitCapsule(
		const CapsuleCollider& capsule1,
		const CapsuleCollider& capsule2);

	bool CapsuleHitCapsule(
		const CapsuleCollider& capsule1,
		const CapsuleCollider& capsule2,
		Vec3& hitPoint);

	// 点と三角形の最近接点
	Vec3 ClosestPointOfPointAndTriangle(
		const Vec3 point,
		const TriangleCollider& triangle);
};
