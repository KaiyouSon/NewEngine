#pragma once
#include "Collider.h"

namespace Collision
{
	// �~�Ɖ~
	bool CircleHitCircle(
		const CircleCollider& circle1,
		const CircleCollider& circle2);

	// ���Ƌ�
	bool SphereHitSphere(
		const SphereCollider& sphere1,
		const SphereCollider& sphere2);

	// �L���[�u�ƃL���[�u
	bool CubeHitCube(
		const CubeCollider& cube1,
		const CubeCollider& cube2);

	// �L���[�u�ƃJ�v�Z��
	bool CubeHitCapsule(
		const CubeCollider& cube,
		const CapsuleCollider& capsule);

	bool CubeHitCapsule(
		const CubeCollider& cube,
		const CapsuleCollider& capsule,
		Vec3& hitPoint);

	// ���ƕ���
	bool SphereHitPlane(
		const SphereCollider& sphere,
		const PlaneCollider& plane,
		Vec3* hitPos = nullptr);

	// ���ƎO�p�`
	bool SphereHitTriangle(
		const SphereCollider& sphere,
		const TriangleCollider& triangle,
		Vec3* hitPos = nullptr);

	// ���ƃJ�v�Z��
	bool SphereHitCapsule(
		const SphereCollider& sphere,
		const CapsuleCollider& capsule);

	// ���C�ƕ���
	bool RayHitPlane(
		const RayCollider& ray,
		const PlaneCollider& plane);

	// ���C�Ƌ�
	bool RayHitSphere(
		const RayCollider& ray,
		const SphereCollider& sphere);

	// �J�v�Z���ƃJ�v�Z��
	bool CapsuleHitCapsule(
		const CapsuleCollider& capsule1,
		const CapsuleCollider& capsule2);

	bool CapsuleHitCapsule(
		const CapsuleCollider& capsule1,
		const CapsuleCollider& capsule2,
		Vec3& hitPoint);

	// �_�ƎO�p�`�̍ŋߐړ_
	Vec3 ClosestPointOfPointAndTriangle(
		const Vec3 point,
		const TriangleCollider& triangle);

};