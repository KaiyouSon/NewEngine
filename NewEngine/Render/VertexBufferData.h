#pragma once
#include "MathUtil.h"
#include "Util.h"

// ���_�f�[�^�N���X
struct VertexPosNormalUv
{
	Vec3 pos;	 // xyz���W
	Vec3 normal; // �@���x�N�g��
	Vec2 uv;	 // uv���W
};

struct VertexPosNormalUvBone
{
	Vec3 pos;	 // xyz���W
	Vec3 normal; // �@���x�N�g��
	Vec2 uv;	 // uv���W
	unsigned int boneIndex[maxBoneIndices];
	float boneWeight[maxBoneIndices];
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