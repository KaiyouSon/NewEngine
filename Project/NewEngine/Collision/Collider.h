#pragma once
#include "MathUtil.h"
#include "ICollider.h"
#include <array>

// ----- 円コライダー ------------------- //
struct CircleCollider : public ICollider
{
	Vec2 centerPos = 0;
	float radius = 0;

	CircleCollider();
	CircleCollider(const Vec2 centerPos, const float radius);
};

// ----- 矩形コライダー ----------------- //
struct SquareCollider : public ICollider
{
	Vec2 centerPos;
	Vec2 size;
	Vec2 leftTop;
	Vec2 leftDown;
	Vec2 rightTop;
	Vec2 rightDown;

	SquareCollider();
	SquareCollider(const Vec2 centerPos, const Vec2 size);
};

// ----- キューブコライダー ------------- //
struct CubeCollider : public ICollider
{
	Vec3 centerPos;
	Vec3 size;
	std::array<Vec3, 8> points;

	CubeCollider();
	CubeCollider(const Vec3 centerPos, const Vec3 size);

	void CalcPoints();
};

// ----- 球コライダー ------------------- //
struct SphereCollider : public ICollider
{
	Vec3 centerPos = 0;
	float radius = 0;

	SphereCollider();
	SphereCollider(const Vec3 centerPos, const float radius);
};

// ----- レイコライダー ----------------- //
struct RayCollider : public ICollider
{
	Vec3 startPos;
	Vec3 dirVec;

	RayCollider();
	RayCollider(const Vec3 startPos, const Vec3 dirVec);
};

// ----- 平面コライダー ------------- //
struct PlaneCollider : public ICollider
{
	Vec3 centerPos;		// 中心座標
	Vec3 normal;		// 法線ベクトル

	PlaneCollider();
	PlaneCollider(const Vec3 centerPos, const Vec3 normal);
};

// ----- 三角形コライダー ----------- //
struct TriangleCollider : public ICollider
{
	Vec3 p0;
	Vec3 p1;
	Vec3 p2;
	Vec3 normal;		// 法線ベクトル

	TriangleCollider();
	TriangleCollider(const Vec3 p0, const Vec3 p1, const Vec3 p2);
};

// ----- カプセルコライダー --------- //
struct CapsuleCollider : public ICollider
{
	Vec3 startPos;
	Vec3 endPos;
	float radius;

	CapsuleCollider();
	CapsuleCollider(const Vec3 startPos, const Vec3 endPos, const float radius);
};
