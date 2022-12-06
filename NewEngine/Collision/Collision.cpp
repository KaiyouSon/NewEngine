#include "Collision.h"
using namespace std;

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
	// ���C�̎n�_�ƌ��_�̋���
	float dis1 = Vec3::Distance(rey.startPos, Vec3::zero);
	// ���b�V���̒��S���W�ƌ��_�̋���
	float dis2 = Vec3::Distance(mesh.centerPos, Vec3::zero);
	// ���C�̎n�_�ƃ��b�V������
	float dist = dis1 - dis2;
	// ���b�V���̖@���ƃ��C�̃x�N�g����cos�Ƃ����߂�
	float cosRadius = Vec3::Dot(mesh.normal * -1, rey.dirVec.Norm());

	// ���������_
	Vec3 inter = rey.startPos + rey.dirVec.Norm() * dist;

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