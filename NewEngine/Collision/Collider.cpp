#include "Collider.h"

// ----- 円コライダー ------------------- //
CircleCollider::CircleCollider() :
	centerPos(0), radius(1)
{
}

CircleCollider::CircleCollider(const Vec2& centerPos, const float& radius) :
	centerPos(centerPos), radius(radius)
{
}

// ----- 球コライダー ------------------- //
SphereCollider::SphereCollider() :
	centerPos(0, 0, 0), radius(1)
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

// ----- 線コライダー ------------------- //
LineCollider::LineCollider() :
	startPos(0, 0, 0), endPos(0, 0, 0)
{
}

LineCollider::LineCollider(const Vec3& startPos, const Vec3& endPos) :
	startPos(startPos), endPos(endPos)
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

TriangleCollider::TriangleCollider() :
	p0(0), p1(0), p2(0), normal(0)
{
}

TriangleCollider::TriangleCollider(const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& normal) :
	p0(p0), p1(p1), p2(p2), normal(normal)
{
}
