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
struct ReyCollider
{
	Vec3 startPos;
	Vec3 dirVec;

	ReyCollider();
	ReyCollider(const Vec3& startPos, const Vec3& dirVec);
};

// ----- 線コライダー ------------------- //
struct LineCollider
{
	Vec3 startPos;
	Vec3 endPos;

	LineCollider();
	LineCollider(const Vec3& startPos, const Vec3& endPos);
};

// ----- メッシュコライダー ------------- //
struct MeshCollider
{
	Vec3 centerPos;		// 中心座標
	Vec3 upperLeftPos;	// 左上座標
	Vec3 upperRightPos;	// 右上座標
	Vec3 lowerLeftPos;	// 左下座標
	Vec3 lowerRightPos;	// 左上座標
	Vec3 normal;		// 法線ベクトル

	MeshCollider();
	MeshCollider(const Vec3& centerPos, const Vec3& scale);
};