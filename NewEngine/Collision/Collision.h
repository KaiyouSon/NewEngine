#pragma once
#include "Collider.h"

class Collision
{
public:
	// 円と円
	static bool CircleHitCircle(
		const CircleCollider& circle1,
		const CircleCollider& circle2);

	// 球と球
	static bool SphereHitSphere(
		const SphereCollider& sphere1,
		const SphereCollider& sphere2);

	// 球と平面
	static bool SphereHitPlane(
		const SphereCollider& sphere,
		const PlaneCollider& plane);

	// 三角形ト平面
	static bool SphereHitTriangle(
		const SphereCollider& sphere,
		const TriangleCollider& triangle);

	// レイと平面
	static bool RayHitPlane(
		const RayCollider& ray,
		const PlaneCollider& plane);

	// 点と三角形の最近接点
	static Vec3 ClosestPointOfPointAndTriangle(
		const Vec3 point,
		const TriangleCollider& triangle);

};