#pragma once
#include "MathUtil.h"

// ----- �~�R���C�_�[ ------------------- //
struct CircleCollider
{
	Vec2 centerPos;
	float radius;

	CircleCollider();
	CircleCollider(const Vec2& centerPos, const float& radius);
};

// ----- ���R���C�_�[ ------------------- //
struct SphereCollider
{
	Vec3 centerPos;
	float radius;

	SphereCollider();
	SphereCollider(const Vec3& centerPos, const float& radius);
};

// ----- ���C�R���C�_�[ ----------------- //
struct ReyCollider
{
	Vec3 startPos;
	Vec3 dirVec;

	ReyCollider();
	ReyCollider(const Vec3& startPos, const Vec3& dirVec);
};

// ----- ���R���C�_�[ ------------------- //
struct LineCollider
{
	Vec3 startPos;
	Vec3 endPos;

	LineCollider();
	LineCollider(const Vec3& startPos, const Vec3& endPos);
};

// ----- ���b�V���R���C�_�[ ------------- //
struct MeshCollider
{
	Vec3 centerPos;		// ���S���W
	Vec3 upperLeftPos;	// ������W
	Vec3 upperRightPos;	// �E����W
	Vec3 lowerLeftPos;	// �������W
	Vec3 lowerRightPos;	// ������W
	Vec3 normal;		// �@���x�N�g��

	MeshCollider();
	MeshCollider(const Vec3& centerPos, const Vec3& scale);
};