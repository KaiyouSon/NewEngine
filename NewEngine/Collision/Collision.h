#pragma once
#include "Collider.h"

class Collision
{
public:
	// �~�Ɖ~
	static bool CircleHitCircle(
		const CircleCollider& circle1,
		const CircleCollider& circle2);

	// ���Ƌ�
	static bool SphereHitSphere(
		const SphereCollider& sphere1,
		const SphereCollider& sphere2);

	// ���ƕ���
	static bool SphereHitPlane(
		const SphereCollider& sphere,
		const PlaneCollider& plane);

	// �O�p�`�g����
	static bool SphereHitTriangle(
		const SphereCollider& sphere,
		const TriangleCollider& triangle);

	// ���C�ƕ���
	static bool RayHitPlane(
		const RayCollider& ray,
		const PlaneCollider& plane);

	// �_�ƎO�p�`�̍ŋߐړ_
	static Vec3 ClosestPointOfPointAndTriangle(
		const Vec3 point,
		const TriangleCollider& triangle);

};