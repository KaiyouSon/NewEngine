#pragma once
#include "MathUtil.h"
#include "Util.h"

namespace VertexBufferData
{
	// 鬆らせ繝・・繧ｿ繧ｯ繝ｩ繧ｹ
	struct VObjModel
	{
		Vec3 pos;	 // xyz蠎ｧ讓・
		Vec3 normal; // 豕慕ｷ壹・繧ｯ繝医Ν
		Vec2 uv;	 // uv蠎ｧ讓・
	};

	struct VFbxModel
	{
		Vec3 pos;	 // xyz蠎ｧ讓・
		Vec3 normal; // 豕慕ｷ壹・繧ｯ繝医Ν
		Vec2 uv;	 // uv蠎ｧ讓・
		unsigned int boneIndex[maxBoneIndices];
		float boneWeight[maxBoneIndices];
	};

	struct VSprite
	{
		Vec3 pos;	 // xyz蠎ｧ讓・
		Vec2 uv;	 // uv蠎ｧ讓・
	};

	struct VLine
	{
		Vec3 pos;	 // xyz蠎ｧ讓・
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
