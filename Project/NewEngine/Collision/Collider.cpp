#include "Collider.h"
#include "Util.h"

// ----- 円コライダー ------------------- //
CircleCollider::CircleCollider() :
	centerPos(0), radius(1)
{
}

CircleCollider::CircleCollider(const Vec2& centerPos, const float& radius) :
	centerPos(centerPos), radius(radius)
{
}

// ----- 矩形コライダー ----------------- //
SquareCollider::SquareCollider() :
	centerPos(0), size(0), leftTop(0), leftDown(0), rightTop(0), rightDown(0)
{
}

SquareCollider::SquareCollider(const Vec2& centerPos, const Vec2& size) :
	centerPos(centerPos), size(size),
	leftTop({ centerPos.x - size.x, centerPos.y - size.y }),
	leftDown({ centerPos.x - size.x, centerPos.y + size.y }),
	rightTop({ centerPos.x + size.x, centerPos.y - size.y }),
	rightDown({ centerPos.x + size.x, centerPos.y + size.y })
{
}

// ----- 球コライダー ------------------- //
SphereCollider::SphereCollider()
{
}
SphereCollider::SphereCollider(const Vec3& centerPos, const float& radius) :
	centerPos(centerPos), radius(radius)
{
}

// ----- レイコライダー ----------------- //
RayCollider::RayCollider() :
	startPos(0, 0, 0), dirVec(0, 0, 1)
{
}
RayCollider::RayCollider(const Vec3& startPos, const Vec3& dirVec) :
	startPos(startPos), dirVec(dirVec)
{
}

// ----- 平面コライダー ------------- //
PlaneCollider::PlaneCollider() :
	centerPos(0, 0, 0), normal(0, 0, 0)
{
}

PlaneCollider::PlaneCollider(const Vec3& centerPos, const Vec3& normal) :
	centerPos(centerPos), normal(normal)
{
}

// ----- 三角形コライダー ----------- //
TriangleCollider::TriangleCollider() :
	p0(0), p1(0), p2(0), normal(0)
{
}

TriangleCollider::TriangleCollider(const Vec3& p0, const Vec3& p1, const Vec3& p2) :
	p0(p0), p1(p1), p2(p2), normal(GetTriangleNormal(p0, p1, p2))
{
}

// ----- カプセルコライダー --------- //
CapsuleCollider::CapsuleCollider() :
	startPos(0), endPos(0), radius(0)
{
}

CapsuleCollider::CapsuleCollider(const Vec3& startPos, const Vec3& endPos, const float& radius) :
	startPos(startPos), endPos(endPos), radius(radius)
{
}

void CubeCollider::CalcPoints()
{
	// 左上前
	points[(uint32_t)CubePoints::FLT] =
	{
		centerPos.x - size.x,
		centerPos.y + size.y,
		centerPos.z + size.z,
	};

	// 左下前
	points[(uint32_t)CubePoints::FLD] =
	{
		centerPos.x - size.x,
		centerPos.y - size.y,
		centerPos.z + size.z,
	};

	// 右上前
	points[(uint32_t)CubePoints::FRT] =
	{
		centerPos.x + size.x,
		centerPos.y + size.y,
		centerPos.z + size.z,
	};

	// 右下前
	points[(uint32_t)CubePoints::FRD] =
	{
		centerPos.x + size.x,
		centerPos.y - size.y,
		centerPos.z + size.z,
	};

	// 左上後
	points[(uint32_t)CubePoints::BLT] =
	{
		centerPos.x - size.x,
		centerPos.y + size.y,
		centerPos.z - size.z,
	};

	// 左下後
	points[(uint32_t)CubePoints::BLD] =
	{
		centerPos.x - size.x,
		centerPos.y - size.y,
		centerPos.z - size.z,
	};

	// 右上後
	points[(uint32_t)CubePoints::BRT] =
	{
		centerPos.x + size.x,
		centerPos.y + size.y,
		centerPos.z - size.z,
	};

	// 右下後
	points[(uint32_t)CubePoints::BRD] =
	{
		centerPos.x + size.x,
		centerPos.y - size.y,
		centerPos.z - size.z,
	};
}
