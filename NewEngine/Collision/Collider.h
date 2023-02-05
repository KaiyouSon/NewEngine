#pragma once
#include "MathUtil.h"

// ----- �~�R���C�_�[ ------------------- //
struct CircleCollider
{
	Vec2 centerPos = 0;
	float radius = 0;

	CircleCollider();
	CircleCollider(const Vec2& centerPos, const float& radius);
};

// ----- ��`�R���C�_�[ ----------------- //
struct SquareCollider
{
	Vec2 centerPos;
	Vec2 size;
	Vec2 leftTop;
	Vec2 leftDown;
	Vec2 rightTop;
	Vec2 rightDown;

	SquareCollider();
	SquareCollider(const Vec2& centerPos, const Vec2& size);
};

// ----- ���R���C�_�[ ------------------- //
struct SphereCollider
{
	Vec3 centerPos = 0;
	float radius = 0;

	SphereCollider();
	SphereCollider(const Vec3& centerPos, const float& radius);
};

// ----- ���C�R���C�_�[ ----------------- //
struct RayCollider
{
	Vec3 startPos;
	Vec3 dirVec;

	RayCollider();
	RayCollider(const Vec3& startPos, const Vec3& dirVec);
};

// ----- ���ʃR���C�_�[ ------------- //
struct PlaneCollider
{
	Vec3 centerPos;		// ���S���W
	Vec3 normal;		// �@���x�N�g��

	PlaneCollider();
	PlaneCollider(const Vec3& centerPos, const Vec3& normal);
};

// ----- �O�p�`�R���C�_�[ ----------- //
struct TriangleCollider
{
	Vec3 p0;
	Vec3 p1;
	Vec3 p2;
	Vec3 normal;		// �@���x�N�g��

	TriangleCollider();
	TriangleCollider(const Vec3& p0, const Vec3& p1, const Vec3& p2);
};

// ----- �J�v�Z���R���C�_�[ --------- //
struct CapsuleCollider
{
	Vec3 startPos;
	Vec3 endPos;
	float radius;

	CapsuleCollider();
	CapsuleCollider(const Vec3& startPos, const Vec3& endPos, const float& radius);
};