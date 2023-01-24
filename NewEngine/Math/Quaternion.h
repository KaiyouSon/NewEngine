#pragma once
#include "Vec3.h"

class Vec3;

class Quaternion
{
public:
	float x, y, z, w;

	// コンストラクタ
	Quaternion() : x(0), y(0), z(0), w(0) {}
	Quaternion(const float& x, const float& y, const float& z) : x(x), y(y), z(z), w(0) {}
	Quaternion(const float& x, const float& y, const float& z, const float& w) : x(x), y(y), z(z), w(w) {}

	// 関数
	float Lenght() const;			// クオータニオンの大きさ
	Quaternion Norm() const;		// 正規化クオータニオン
	Quaternion Conjugate() const;	// 共役クオータニオン
	Quaternion Inverse() const;		// 逆クオータニオン

	// 任意軸回転
	Quaternion AnyAxisRotation(const Vec3& v, const float& radian);

	static float Dot(const Quaternion& q1, const Quaternion& q2);		// 二つのクオータニオンの内積
	static Quaternion Identity();	// 単位クオータニオン
	static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
	static Quaternion MakeAxisAngle(const Vec3& v, const float& radian);


	Quaternion operator-() const;

	// 算術演算子のオーバーロード
	Quaternion operator+(const Quaternion& other) const;	// もう一つのクオータニオンとの足し算
	Quaternion operator-(const Quaternion& other) const;	// もう一つのクオータニオンとの引き算
	Quaternion operator*(const Quaternion& other) const;	// もう一つのクオータニオンとの掛け算
	Quaternion operator/(const Quaternion& other) const;	// もう一つのクオータニオンとの割り算
	Quaternion operator*(const float& num) const;			// 一つの値との掛け算
	Quaternion operator/(const float& num) const;			// 一つの値との割り算

	// 複合代入演算子のオーバーロード
	Quaternion& operator+=(const Quaternion& other);	// 複合代入演算 +=
	Quaternion& operator-=(const Quaternion& other);	// 複合代入演算 -=
	Quaternion& operator*=(const Quaternion& other);	// 複合代入演算 *=
	Quaternion& operator*=(const float& num);			// 複合代入演算 *=
	Quaternion& operator/=(const float& num);			// 複合代入演算 /=
};

Quaternion operator*(const float& num, const Quaternion& q);