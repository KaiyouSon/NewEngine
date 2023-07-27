#include "Collision.h"
#include "Util.h"

// �~�Ɖ~
bool Collision::CircleHitCircle(const CircleCollider& circle1, const CircleCollider& circle2)
{
	return
	{
		(circle2.centerPos.x - circle1.centerPos.x) *
		(circle2.centerPos.x - circle1.centerPos.x) +
		(circle2.centerPos.y - circle1.centerPos.y) *
		(circle2.centerPos.y - circle1.centerPos.y) <=
		(circle2.radius + circle1.radius) *
		(circle2.radius + circle1.radius)
	};
}

// ���Ƌ�
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

// �L���[�u�ƃL���[�u
bool Collision::CubeHitCube(const CubeCollider& cube1, const CubeCollider& cube2)
{
	// �d�Ȃ蔻��
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			bool isXHit =
				std::fabs(cube1.points[i].x - cube2.points[j].x) <= (cube1.size.x + cube2.size.x) / 2.0;

			bool isYHit =
				std::fabs(cube1.points[i].y - cube2.points[j].y) <= (cube1.size.y + cube2.size.y) / 2.0;

			bool isZHit =
				std::fabs(cube1.points[i].z - cube2.points[j].z) <= (cube1.size.z + cube2.size.z) / 2.0;

			if (isXHit && isYHit && isZHit)
			{
				return true;  // �d�Ȃ肪����ꍇ�͓�����Ƃ���
			}
		}
	}

	return false;  // �d�Ȃ肪�Ȃ��ꍇ�͓�����Ƃ��Ȃ�
}

// ���ƕ���
bool Collision::SphereHitPlane(const SphereCollider& sphere, const PlaneCollider& plane, Vec3* hitPos)
{
	// ���W�n�̌��_���狅�̒��S���W�ւ̋���
	float dot1 = Vec3::Dot(sphere.centerPos, plane.normal);
	// ���ʂ̌��_����
	float dot2 = Vec3::Dot(plane.centerPos, plane.normal);
	float dis = dot1 - dot2;

	if (fabsf(dis) > sphere.radius)
	{
		return false;
	}

	if (hitPos != nullptr)
	{
		*hitPos = dis * plane.normal + sphere.centerPos;
	}

	return true;
}

// ���ƎO�p�`
bool Collision::SphereHitTriangle(const SphereCollider& sphere, const TriangleCollider& triangle, Vec3* hitPos)
{
	Vec3 p = ClosestPointOfPointAndTriangle(sphere.centerPos, triangle);

	Vec3 v = p - sphere.centerPos;

	float value = Vec3::Dot(v, v);

	if (value > sphere.radius * sphere.radius)
	{
		return false;
	}

	if (hitPos != nullptr)
	{
		*hitPos = p;
	}

	return true;
}

// ���ƃJ�v�Z��
bool Collision::SphereHitCapsule(const SphereCollider& sphere, const CapsuleCollider& capsule)
{
	// �J�v�Z���̎��x�N�g���Ƌ��̒��S�ʒu�x�N�g���̍����v�Z
	Vec3 capsuleAxis = capsule.endPos - capsule.startPos;
	Vec3 sphereToCapsule = sphere.centerPos - capsule.startPos;

	Vec3 n = capsuleAxis.Norm();

	// �J�v�Z���̎��x�N�g����̍ŋߐړ_���v�Z
	float t =
		(sphereToCapsule.x * n.x) +
		(sphereToCapsule.y * n.y) +
		(sphereToCapsule.z * n.z);

	Vec3 closestPoint;
	if (t <= 0.0f)
	{
		closestPoint = capsule.startPos;
	}
	else if (t >= capsuleAxis.Length())
	{
		closestPoint = capsule.endPos;
	}
	else
	{
		closestPoint = capsule.startPos + capsuleAxis * t;
	}

	// �ŋߐړ_�Ƌ��̒��S�ʒu�̋������v�Z
	float distance =
		(closestPoint.x - sphere.centerPos.x) * (closestPoint.x - sphere.centerPos.x) +
		(closestPoint.y - sphere.centerPos.y) * (closestPoint.y - sphere.centerPos.y) +
		(closestPoint.z - sphere.centerPos.z) * (closestPoint.z - sphere.centerPos.z);

	// ���������a�̍��v�����������ꍇ�͏Փ˂��Ă���Ƃ݂Ȃ�
	float totalRadius = sphere.radius + sphere.radius;

	if (distance <= (totalRadius * totalRadius))
	{
		return true;
	}

	return false;
}

// ���C�ƕ���
bool Collision::RayHitPlane(const RayCollider& ray, const PlaneCollider& plane)
{
	const float epsilon = 1.0e-5f;

	// ���W�n�̌��_���烌�C�̎n�_���W�ւ̋���
	float dot1 = Vec3::Dot(plane.normal, ray.dirVec);

	// ���ʔ���
	if (dot1 > -epsilon)
	{
		return false;
	}

	// ���ʂ̌��_����
	float dot2 = Vec3::Dot(plane.normal, ray.startPos);

	float dis = dot2 - Vec3::Dot(plane.centerPos, plane.normal);

	float t = dis / -dot1;
	if (t < 0)
	{
		return false;
	}

	return true;
}

// ���C�Ƌ�
bool Collision::RayHitSphere(const RayCollider& ray, const SphereCollider& sphere)
{
	Vec3 v = ray.startPos - sphere.centerPos;
	float dot1 = Vec3::Dot(v, ray.dirVec);
	float dot2 = Vec3::Dot(v, v) - (sphere.radius * sphere.radius);

	// ���C�������痣��Ă����������w���Ă���ꍇ(b > 0)�A���C�̎n�_��sphere�̊O���ɂ���(c > 0)
	if (dot1 > 0.f && dot2 > 0.f)
	{
		return false;
	}

	// ���̔��ʎ��̓��C�������O��Ă��邱�ƂɈ�v
	float discr = (dot1 * dot1) - dot2;
	if (discr < 0.f)
	{
		return false;
	}

	return true;
}

bool Collision::CapsuleHitCapsule(const CapsuleCollider& capsule1, const CapsuleCollider& capsule2)
{
	Vec3 empty = 0;
	return CapsuleHitCapsule(capsule1, capsule2, empty);
}

// �J�v�Z���ƃJ�v�Z��
bool Collision::CapsuleHitCapsule(const CapsuleCollider& capsule1, const CapsuleCollider& capsule2, Vec3& hitPoint)
{
	Vec3 d1 = capsule1.endPos - capsule1.startPos;
	Vec3 d2 = capsule2.endPos - capsule2.startPos;

	Vec3 r = capsule1.startPos - capsule2.startPos;

	float a = Vec3::Dot(d1, d1);
	float b = Vec3::Dot(d1, d2);
	float e = Vec3::Dot(d2, d2);

	float c = Vec3::Dot(d1, r);
	float f = Vec3::Dot(d2, r);

	float s = 0.f;
	float t = 0.f;

	float denominator = a * e - b * b;

	// s �̒l�����߂�
	if (denominator != 0.0f)
	{
		s = (b * f - c * e) / denominator;
		s = Clamp(s);
	}
	else
	{
		s = 0;
	}

	// t �̒l�����߂�
	t = (f + b * s) / e;

	// t ��[ 0.0, 1.0 ]�͈̔͂ł���΁As ���Čv�Z
	if (t < 0.0f)
	{
		t = 0.0f;
		s = Clamp(-c / a);
	}
	else if (t > 1.0f)
	{
		s = Clamp((b - c) / a);
		t = 1.0f;
	}

	Vec3 c1 = capsule1.startPos + d1 * s;
	Vec3 c2 = capsule2.startPos + d2 * t;

	float disPow2 =
		((c2.x - c1.x) * (c2.x - c1.x)) +
		((c2.y - c1.y) * (c2.y - c1.y)) +
		((c2.z - c1.z) * (c2.z - c1.z));

	// ��̃J�v�Z���̔��a�̘a
	float radius = capsule1.radius + capsule2.radius;

	if (disPow2 < radius * radius)
	{
		// Calculate the collision point
		float distance = sqrt(disPow2);
		float penetration = radius - distance;

		Vec3 direction = (c2 - c1) / distance;
		hitPoint = c1 + direction * (capsule1.radius - 0.5f * penetration);

		return true;
	}


	if (disPow2 < radius * radius)
	{
		return true;
	}

	return false;
}

// �_�ƎO�p�`�̍ŋߐړ_
Vec3 Collision::ClosestPointOfPointAndTriangle(const Vec3 point, const TriangleCollider& triangle)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vec3 p0_p1 = triangle.p1 - triangle.p0;
	Vec3 p0_p2 = triangle.p2 - triangle.p0;
	Vec3 p0_pt = point - triangle.p0;

	float d1 = Vec3::Dot(p0_p1, p0_pt);
	float d2 = Vec3::Dot(p0_p2, p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0���ŋߖT
		return triangle.p0;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vec3 p1_pt = point - triangle.p1;

	float d3 = Vec3::Dot(p0_p1, p1_pt);
	float d4 = Vec3::Dot(p0_p2, p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1���ŋߖT
		return triangle.p1;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		return triangle.p0 + p0_p1 * v;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vec3 p2_pt = point - triangle.p2;

	float d5 = Vec3::Dot(p0_p1, p2_pt);
	float d6 = Vec3::Dot(p0_p2, p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		return triangle.p2;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		return triangle.p0 + p0_p2 * w;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return triangle.p1 + (triangle.p2 - triangle.p1) * w;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;

	return 0;
}
