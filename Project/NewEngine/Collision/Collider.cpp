#include "Collider.h"
#include "Util.h"

// ----- �~�R���C�_�[ ------------------- //
CircleCollider::CircleCollider() :
	centerPos(0), radius(1)
{
	primitive = ColliderPrimitive::Sphere;
}
CircleCollider::CircleCollider(const Vec2 centerPos, const float radius) :
	centerPos(centerPos), radius(radius)
{
	primitive = ColliderPrimitive::Sphere;
}

// ----- ��`�R���C�_�[ ----------------- //
SquareCollider::SquareCollider() :
	centerPos(0), size(0), leftTop(0), leftDown(0), rightTop(0), rightDown(0)
{
	primitive = ColliderPrimitive::Square;
}
SquareCollider::SquareCollider(const Vec2 centerPos, const Vec2 size) :
	centerPos(centerPos), size(size),
	leftTop({ centerPos.x - size.x, centerPos.y - size.y }),
	leftDown({ centerPos.x - size.x, centerPos.y + size.y }),
	rightTop({ centerPos.x + size.x, centerPos.y - size.y }),
	rightDown({ centerPos.x + size.x, centerPos.y + size.y })
{
	primitive = ColliderPrimitive::Square;
}

// ----- �L���[�u�R���C�_�[ ------------- //
CubeCollider::CubeCollider() :
	centerPos(0), size(0)
{
	primitive = ColliderPrimitive::Cube;
}
CubeCollider::CubeCollider(const Vec3 centerPos, const Vec3 size) :
	centerPos(centerPos), size(size)
{
	primitive = ColliderPrimitive::Cube;
}
void CubeCollider::CalcPoints()
{
	// ����O
	points[(uint32_t)CubePoints::FLT] =
	{
		centerPos.x - size.x,
		centerPos.y + size.y,
		centerPos.z + size.z,
	};

	// �����O
	points[(uint32_t)CubePoints::FLD] =
	{
		centerPos.x - size.x,
		centerPos.y - size.y,
		centerPos.z + size.z,
	};

	// �E��O
	points[(uint32_t)CubePoints::FRT] =
	{
		centerPos.x + size.x,
		centerPos.y + size.y,
		centerPos.z + size.z,
	};

	// �E���O
	points[(uint32_t)CubePoints::FRD] =
	{
		centerPos.x + size.x,
		centerPos.y - size.y,
		centerPos.z + size.z,
	};

	// �����
	points[(uint32_t)CubePoints::BLT] =
	{
		centerPos.x - size.x,
		centerPos.y + size.y,
		centerPos.z - size.z,
	};

	// ������
	points[(uint32_t)CubePoints::BLD] =
	{
		centerPos.x - size.x,
		centerPos.y - size.y,
		centerPos.z - size.z,
	};

	// �E���
	points[(uint32_t)CubePoints::BRT] =
	{
		centerPos.x + size.x,
		centerPos.y + size.y,
		centerPos.z - size.z,
	};

	// �E����
	points[(uint32_t)CubePoints::BRD] =
	{
		centerPos.x + size.x,
		centerPos.y - size.y,
		centerPos.z - size.z,
	};
}

// ----- ���R���C�_�[ ------------------- //
SphereCollider::SphereCollider() :
	centerPos(0), radius(0)
{
	primitive = ColliderPrimitive::Sphere;
}
SphereCollider::SphereCollider(const Vec3 centerPos, const float radius) :
	centerPos(centerPos), radius(radius)
{
	primitive = ColliderPrimitive::Sphere;
}

// ----- ���C�R���C�_�[ ----------------- //
RayCollider::RayCollider() :
	startPos(0, 0, 0), dirVec(0, 0, 1)
{
	primitive = ColliderPrimitive::Ray;
}
RayCollider::RayCollider(const Vec3 startPos, const Vec3 dirVec) :
	startPos(startPos), dirVec(dirVec)
{
	primitive = ColliderPrimitive::Ray;
}

// ----- ���ʃR���C�_�[ ----------------- //
PlaneCollider::PlaneCollider() :
	centerPos(0, 0, 0), normal(0, 0, 0)
{
	primitive = ColliderPrimitive::Plane;
}
PlaneCollider::PlaneCollider(const Vec3 centerPos, const Vec3 normal) :
	centerPos(centerPos), normal(normal)
{
	primitive = ColliderPrimitive::Plane;
}

// ----- �O�p�`�R���C�_�[ --------------- //
TriangleCollider::TriangleCollider() :
	p0(0), p1(0), p2(0), normal(0)
{
	primitive = ColliderPrimitive::Triangle;
}
TriangleCollider::TriangleCollider(const Vec3 p0, const Vec3 p1, const Vec3 p2) :
	p0(p0), p1(p1), p2(p2), normal(GetTriangleNormal(p0, p1, p2))
{
	primitive = ColliderPrimitive::Triangle;
}

// ----- �J�v�Z���R���C�_�[ ------------- //
CapsuleCollider::CapsuleCollider() :
	startPos(0), endPos(0), radius(0)
{
	primitive = ColliderPrimitive::Capsule;
}
CapsuleCollider::CapsuleCollider(const Vec3 startPos, const Vec3 endPos, const float radius) :
	startPos(startPos), endPos(endPos), radius(radius)
{
	primitive = ColliderPrimitive::Capsule;
}
