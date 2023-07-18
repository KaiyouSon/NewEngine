#pragma once
#include "MathUtil.h"
#include "ICollider.h"
#include <array>

// ----- �~�R���C�_�[ ------------------- //
struct CircleCollider : public ICollider
{
	Vec2 centerPos = 0;
	float radius = 0;

	CircleCollider();
	CircleCollider(const Vec2 centerPos, const float radius);
};

// ----- ��`�R���C�_�[ ----------------- //
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

// ----- �L���[�u�R���C�_�[ ------------- //
struct CubeCollider : public ICollider
{
	Vec3 centerPos;
	Vec3 size;
	std::array<Vec3, 8> points;

	CubeCollider();
	CubeCollider(const Vec3 centerPos, const Vec3 size);

	void CalcPoints();
};

// ----- ���R���C�_�[ ------------------- //
struct SphereCollider : public ICollider
{
	Vec3 centerPos = 0;
	float radius = 0;

	SphereCollider();
	SphereCollider(const Vec3 centerPos, const float radius);
};

// ----- ���C�R���C�_�[ ----------------- //
struct RayCollider : public ICollider
{
	Vec3 startPos;
	Vec3 dirVec;

	RayCollider();
	RayCollider(const Vec3 startPos, const Vec3 dirVec);
};

// ----- ���ʃR���C�_�[ ------------- //
struct PlaneCollider : public ICollider
{
	Vec3 centerPos;		// ���S���W
	Vec3 normal;		// �@���x�N�g��

	PlaneCollider();
	PlaneCollider(const Vec3 centerPos, const Vec3 normal);
};

// ----- �O�p�`�R���C�_�[ ----------- //
struct TriangleCollider : public ICollider
{
	Vec3 p0;
	Vec3 p1;
	Vec3 p2;
	Vec3 normal;		// �@���x�N�g��

	TriangleCollider();
	TriangleCollider(const Vec3 p0, const Vec3 p1, const Vec3 p2);
};

// ----- �J�v�Z���R���C�_�[ --------- //
struct CapsuleCollider : public ICollider
{
	Vec3 startPos;
	Vec3 endPos;
	float radius;

	CapsuleCollider();
	CapsuleCollider(const Vec3 startPos, const Vec3 endPos, const float radius);
};