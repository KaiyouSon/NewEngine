#pragma once
#include "MathUtil.h"

// ----- 円コライダー ------------------- //
struct CircleCollider
{
	Vec2 centerPos;
	float radius;

	CircleCollider();
	CircleCollider(const Vec2& centerPos, const float& radius);
};

// ----- 球コライダー ------------------- //
struct SphereCollider
{
	Vec3 centerPos;
	float radius;

	SphereCollider();
	SphereCollider(const Vec3& centerPos, const float& radius);
};

// ----- レイコライダー ----------------- //
struct RayCollider
{
	Vec3 startPos;
	Vec3 dirVec;

	RayCollider();
	RayCollider(const Vec3& startPos, const Vec3& dirVec);
};

// ----- 線コライダー ------------------- //
struct LineCollider
{
	Vec3 startPos;
	Vec3 endPos;

	LineCollider();
	LineCollider(const Vec3& startPos, const Vec3& endPos);
};

// ----- 平面コライダー ------------- //
struct PlaneCollider
{
	Vec3 centerPos;		// 中心座標
	Vec3 normal;		// 法線ベクトル

	PlaneCollider();
	PlaneCollider(const Vec3& centerPos, const Vec3& normal);
};

// ----- 三角形コライダー ----------- //
struct TriangleCollider
{
	Vec3 p0;
	Vec3 p1;
	Vec3 p2;
	Vec3 normal;		// 法線ベクトル

	TriangleCollider();
	TriangleCollider(const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& normal);
};