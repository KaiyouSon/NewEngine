#pragma once
#include "MathUtil.h"
#include "Color.h"

// ���_�f�[�^�N���X
struct VertexPosNormalUv
{
	Vec3 pos;	 // xyz���W
	Vec3 normal; // �@���x�N�g��
	Vec2 uv;	 // uv���W
};

struct VertexPosUv
{
	Vec3 pos;	 // xyz���W
	Vec2 uv;	 // uv���W
};

struct VertexPos
{
	Vec3 pos;	 // xyz���W
};

struct VertexParticle
{
	Vec3 pos;
	float scale;
	Color color;
};