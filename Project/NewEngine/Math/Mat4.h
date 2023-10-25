#pragma once
#include "Vec3.h"

// 4x4行列の構造体
struct Mat4
{
	float m[4][4] = {};

	Mat4();
	Mat4(
		float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33);

	static Mat4 Zero();		// 繧ｼ繝ｭ陦悟・繧定ｿ斐☆髢｢謨ｰ
	static Mat4 Identity();	// 蜊倅ｽ崎｡悟・繧定ｿ斐☆髢｢謨ｰ
	Mat4 Inverse();			// 騾・｡悟・繧定ｿ斐☆髢｢謨ｰ
	Mat4 Transpose();

	void SetXAxis(const Vec3 xAxis);
	void SetYAxis(const Vec3 yAxis);
	void SetZAxis(const Vec3 zAxis);
	void SetTranslation(const Vec3 pos);

	Vec3 GetXAxis() const;
	Vec3 GetYAxis() const;
	Vec3 GetZAxis() const;
	Vec3 GetTrans() const;
	Vec3 GetScale() const;

	// 蜊倬・ｼ皮ｮ怜ｭ舌が繝ｼ繝舌・繝ｭ繝ｼ繝・
	Mat4 operator*(const Mat4& other) const;
	Mat4 operator*(const float num);

	// 莉｣蜈･貍皮ｮ怜ｭ舌が繝ｼ繝舌・繝ｭ繝ｼ繝・
	Mat4& operator*=(const Mat4& other);

	// 豈碑ｼ・ｼ皮ｮ怜ｭ舌・繧ｪ繝ｼ繝舌・繝ｭ繝ｼ繝・
	bool operator!=(const Mat4& other) const;

};
