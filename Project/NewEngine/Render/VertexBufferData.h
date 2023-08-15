#pragma once
#include "MathUtil.h"
#include "Util.h"

namespace VertexBufferData
{
	// 頂点データクラス
	struct VObjModel
	{
		Vec3 pos;	 // xyz座標
		Vec3 normal; // 法線ベクトル
		Vec2 uv;	 // uv座標
	};

	struct VFbxModel
	{
		Vec3 pos;	 // xyz座標
		Vec3 normal; // 法線ベクトル
		Vec2 uv;	 // uv座標
		unsigned int boneIndex[maxBoneIndices];
		float boneWeight[maxBoneIndices];
	};

	struct VSprite
	{
		Vec3 pos;	 // xyz座標
		Vec2 uv;	 // uv座標
	};

	struct VLine
	{
		Vec3 pos;	 // xyz座標
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
}