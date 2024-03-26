#pragma once

// 前方宣言
struct Vec3;

// クォーターニオンの構造体
struct Quaternion
{
	float x, y, z, w;

	// クォータニオンのデフォルトコンストラクタ
	Quaternion() : x(0), y(0), z(0), w(0) {}
	Quaternion(const Vec3 v);
	Quaternion(const float x, const float y, const float z) : x(x), y(y), z(z), w(0) {}
	Quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}

	// 長さを計算
	float Length() const;            // クォータニオンの長さを返す
	Quaternion Norm() const;        // 正規化されたクォータニオンを返す
	Quaternion Conjugate() const;    // 共役クォータニオンを返す
	Quaternion Inverse() const;        // 逆クォータニオンを返す

	// 任意軸回転
	Quaternion AnyAxisRotation(const Vec3 v, const float radian);

	static float Dot(const Quaternion q1, const Quaternion q2);        // ドット積を計算
	static Quaternion Identity();    // 単位クォータニオンを返す
	static Quaternion Slerp(const Quaternion q1, const Quaternion q2, float t);
	static Quaternion MakeAxisAngle(const Vec3 v, const float radian);
	static Quaternion DirectionToDirection(const Vec3 v1, const Vec3 v2);
	static Quaternion Lerp(const Quaternion s, const Quaternion e, const float f);
	static Vec3 RotateVector(const Quaternion& q, const Vec3& v);

	Quaternion operator-() const;

	// 二項演算子
	Quaternion operator+(const Quaternion other) const;    // 加算
	Quaternion operator-(const Quaternion other) const;    // 減算
	Quaternion operator*(const Quaternion other) const;    // 乗算
	Quaternion operator*(const float num) const;            // スカラー乗算
	Quaternion operator/(const float num) const;            // スカラー除算

	// 複合代入演算子
	Quaternion& operator+=(const Quaternion other);    // 加算代入
	Quaternion& operator-=(const Quaternion other);    // 減算代入
	Quaternion& operator*=(const Quaternion other);    // 乗算代入
	Quaternion& operator*=(const float num);            // スカラー乗算代入
	Quaternion& operator/=(const float num);            // スカラー除算代入
};

Quaternion operator*(const float num, const Quaternion q);
