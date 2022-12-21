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

	// レイとメッシュ
	static bool ReyHitMesh(
		const ReyCollider& rey,
		const MeshCollider& mesh);

	// 線とメッシュ
	static bool LineHitMesh(
		const LineCollider& line,
		const MeshCollider& mesh);
};