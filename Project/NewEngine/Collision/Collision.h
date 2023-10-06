#pragma once
#include "Collider.h"

namespace Collision
{
	// 蜀・→蜀・
	bool CircleHitCircle(
		const CircleCollider& circle1,
		const CircleCollider& circle2);

	// 逅・→逅・
	bool SphereHitSphere(
		const SphereCollider& sphere1,
		const SphereCollider& sphere2);

	// 繧ｭ繝･繝ｼ繝悶→繧ｭ繝･繝ｼ繝・
	bool CubeHitCube(
		const CubeCollider& cube1,
		const CubeCollider& cube2);

	// 繧ｭ繝･繝ｼ繝悶→繧ｫ繝励そ繝ｫ
	bool CubeHitCapsule(
		CubeCollider& cube,
		CapsuleCollider& capsule);

	bool CubeHitCapsule(
		const CubeCollider& cube,
		const CapsuleCollider& capsule,
		Vec3& hitPoint);

	// 逅・→蟷ｳ髱｢
	bool SphereHitPlane(
		const SphereCollider& sphere,
		const PlaneCollider& plane,
		Vec3* hitPoint = nullptr);

	// 逅・→荳芽ｧ貞ｽ｢
	bool SphereHitTriangle(
		const SphereCollider& sphere,
		const TriangleCollider& triangle,
		Vec3* hitPoint = nullptr);

	// 逅・→繧ｫ繝励そ繝ｫ
	bool SphereHitCapsule(
		const SphereCollider& sphere,
		const CapsuleCollider& capsule);

	bool SphereHitCapsule(
		const SphereCollider& sphere,
		const CapsuleCollider& capsule,
		Vec3& hitPoint);

	// 繝ｬ繧､縺ｨ蟷ｳ髱｢
	bool RayHitPlane(
		const RayCollider& ray,
		const PlaneCollider& plane);

	// 繝ｬ繧､縺ｨ逅・
	bool RayHitSphere(
		const RayCollider& ray,
		const SphereCollider& sphere);

	// 繧ｫ繝励そ繝ｫ縺ｨ繧ｫ繝励そ繝ｫ
	bool CapsuleHitCapsule(
		const CapsuleCollider& capsule1,
		const CapsuleCollider& capsule2);

	bool CapsuleHitCapsule(
		const CapsuleCollider& capsule1,
		const CapsuleCollider& capsule2,
		Vec3& hitPoint);

	// 轤ｹ縺ｨ荳芽ｧ貞ｽ｢縺ｮ譛霑第磁轤ｹ
	Vec3 ClosestPointOfPointAndTriangle(
		const Vec3 point,
		const TriangleCollider& triangle);

};
