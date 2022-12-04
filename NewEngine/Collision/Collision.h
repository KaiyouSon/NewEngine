#pragma once
#include "Collider.h"

class Collision
{
public:
	// ���Ƌ�
	static bool SphereHitSphere(
		const SphereCollider& sphere1,
		const SphereCollider& sphere2);

	// ���C�ƃ��b�V��
	static bool ReyHitMesh(
		const ReyCollider& rey,
		const MeshCollider& mesh);

	// ���ƃ��b�V��
	static bool LineHitMesh(
		const LineCollider& line,
		const MeshCollider& mesh);
};