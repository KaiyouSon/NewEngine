#pragma once
#include "MathUtil.h"
#include "Color.h"

// 頂点データクラス
struct VertexPosNormalUv
{
	Vec3 pos;	 // xyz座標
	Vec3 normal; // 法線ベクトル
	Vec2 uv;	 // uv座標
};

struct VertexPosUv
{
	Vec3 pos;	 // xyz座標
	Vec2 uv;	 // uv座標
};

struct VertexPos
{
	Vec3 pos;	 // xyz座標
};

struct VertexParticle
{
	Vec3 pos;
	float scale;
	Color color;
};