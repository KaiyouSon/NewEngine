#pragma once
#include "Collider.h"

namespace Collision
{
	// 円と円
	bool CircleHitCircle(
		const CircleCollider& circle1,
		const CircleCollider& circle2);

	// 球と球
	bool SphereHitSphere(
		const SphereCollider& sphere1,
		const SphereCollider& sphere2);

	// キューブとキューブ
	bool CubeHitCube(
		const CubeCollider& cube1,
		const CubeCollider& cube2);

	// キューブとカプセル
	bool CubeHitCapsule(
		const CubeCollider& cube,
		const CapsuleCollider& capsule);

	bool CubeHitCapsule(
		const CubeCollider& cube,
		const CapsuleCollider& capsule,
		Vec3& hitPoint);

	// 球と平面
	bool SphereHitPlane(
		const SphereCollider& sphere,
		const PlaneCollider& plane,
		Vec3* hitPos = nullptr);

	// 球と三角形
	bool SphereHitTriangle(
		const SphereCollider& sphere,
		const TriangleCollider& triangle,
		Vec3* hitPos = nullptr);

	// 球とカプセル
	bool SphereHitCapsule(
		const SphereCollider& sphere,
		const CapsuleCollider& capsule);

	// レイと平面
	bool RayHitPlane(
		const RayCollider& ray,
		const PlaneCollider& plane);

	// レイと球
	bool RayHitSphere(
		const RayCollider& ray,
		const SphereCollider& sphere);

	// カプセルとカプセル
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