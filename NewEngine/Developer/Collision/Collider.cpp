#include "Collider.h"

// ----- 球コライダー ------------------- //
SphereCollider::SphereCollider() :
	centerPos(0, 0, 0), radius(1)
{
	colliderType = SphereShape;
	isDestroy = false;
}
SphereCollider::SphereCollider(Vec3 centerPos, float radius) :
	centerPos(centerPos), radius(radius)
{
	colliderType = SphereShape;
	isDestroy = false;
}

// ----- レイコライダー ----------------- //
ReyCollider::ReyCollider() :
	startPos(0, 0, 0), dirVec(0, 0, 1)
{
	colliderType = ReyShape;
}
ReyCollider::ReyCollider(const Vec3& startPos, const Vec3& dirVec) :
	startPos(startPos), dirVec(dirVec)
{
	colliderType = ReyShape;
	isDestroy = false;
}

// ----- 線コライダー ------------------- //
LineCollider::LineCollider() :
	startPos(0, 0, 0), endPos(0, 0, 0)
{
	colliderType = LineShape;
	isDestroy = false;
}

LineCollider::LineCollider(const Vec3& startPos, const Vec3& endPos) :
	startPos(startPos), endPos(endPos)
{
	colliderType = LineShape;
	isDestroy = false;
}

// ----- メッシュコライダー ------------- //
MeshCollider::MeshCollider() :
	centerPos(0, 0, 0),
	upperLeftPos(0, 0, 0), upperRightPos(0, 0, 0),
	lowerLeftPos(0, 0, 0), lowerRightPos(0, 0, 0),
	normal(0, 0, 0)
{
	colliderType = MeshShape;
	isDestroy = false;
}

MeshCollider::MeshCollider(const Vec3& centerPos, const Vec3& scale) :
	centerPos(centerPos),
	upperLeftPos(centerPos.x - scale.x, centerPos.y + scale.y, centerPos.z),
	upperRightPos(centerPos.x + scale.x, centerPos.y + scale.y, centerPos.z),
	lowerLeftPos(centerPos.x - scale.x, centerPos.y - scale.y, centerPos.z),
	lowerRightPos(centerPos.x + scale.x, centerPos.y - scale.y, centerPos.z),
	normal(Vec3::Cross(
		(lowerRightPos - lowerLeftPos).Normalized(),
		(upperLeftPos - lowerLeftPos).Normalized()))
{
	colliderType = MeshShape;
	isDestroy = false;
}