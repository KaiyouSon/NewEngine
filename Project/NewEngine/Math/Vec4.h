#pragma once

// 前方宣言
struct Vec3;

// ベクター4の構造体
struct Vec4
{
	// 四次元ベクトル
	float x; // ベクトルの x 要素
	float y; // ベクトルの y 要素
	float z; // ベクトルの z 要素
	float w; // ベクトルの w 要素

	// コンストラクタ
	Vec4();
	Vec4(const float num);
	Vec4(const Vec3 vec3);
	Vec4(const Vec3 vec3, const float num);
	Vec4(const float x, const float y, const float z, const float w);
};

