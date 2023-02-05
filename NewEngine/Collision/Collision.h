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
		const PlaneCollider& plane,
		Vec3* hitPos = nullptr);

	// ���ƎO�p�`
	static bool SphereHitTriangle(
		const SphereCollider& sphere,
		const TriangleCollider& triangle,
		Vec3* hitPos = nullptr);

	// ���ƃJ�v�Z��
	static bool SphereHitCapsule(
		const SphereCollider& sphere,
		const CapsuleCollider& capsule);

	// ���C�ƕ���
	static bool RayHitPlane(
		const RayCollider& ray,
		const PlaneCollider& plane);

	// ���C�Ƌ�
	static bool RayHitSphere(
		const RayCollider& ray,
		const SphereCollider& sphere);

	// �J�v�Z���ƃJ�v�Z��
	static bool CapsuleHitCapsule(
		const CapsuleCollider& capsule1,
		const CapsuleCollider& capsule2);

	// �_�ƎO�p�`�̍ŋߐړ_
	static Vec3 ClosestPointOfPointAndTriangle(
		const Vec3 point,
		const TriangleCollider& triangle);

};