#pragma once
#include "MathUtil.h"
#include "Util.h"

namespace VertexBufferData
{
	// ���_�f�[�^�N���X
	struct VObjModel
	{
		Vec3 pos;	 // xyz���W
		Vec3 normal; // �@���x�N�g��
		Vec2 uv;	 // uv���W
	};

	struct VFbxModel
	{
		Vec3 pos;	 // xyz���W
		Vec3 normal; // �@���x�N�g��
		Vec2 uv;	 // uv���W
		unsigned int boneIndex[maxBoneIndices];
		float boneWeight[maxBoneIndices];
	};

	struct VSprite
	{
		Vec3 pos;	 // xyz���W
		Vec2 uv;	 // uv���W
	};

	struct VLine
	{
		Vec3 pos;	 // xyz���W
	};

	struct VParticle
	{
		Vec3 pos;
		float scale;
		Color color;
	};
}