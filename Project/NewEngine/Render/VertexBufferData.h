#pragma once
#include "MathUtil.h"
#include "Util.h"

namespace VertexBufferData
{
	struct VObjModel
	{
		Vec3 pos;
		Vec3 normal;
		Vec2 uv;
	};

	struct VFbxModel
	{
		Vec3 pos;
		Vec3 normal;
		Vec2 uv;
		uint32_t boneIndex[maxBoneIndices];
		float boneWeight[maxBoneIndices];
	};

	struct VVolumetricFog
	{
		Vec3 pos;
		Vec3 normal;
		Vec3 uv;
	};

	struct VSprite
	{
		Vec3 pos;
		Vec2 uv;
	};

	struct VLine
	{
		Vec3 pos;
	};

	struct VParticle
	{
		Vec3 pos;
		Vec2 scale;
		float rot;
		float shininess = 1.0f;
		Color color = Color::zero;
	};

	struct VBranch
	{
		Vec3 pos;
		Vec2 scale;
		Vec3 rot;
	};

	struct VGrass
	{
		Vec3 pos;
		Vec2 scale;
		Vec2 timer;
	};

}
