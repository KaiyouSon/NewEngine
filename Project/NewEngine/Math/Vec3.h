#pragma once
#include "Vec2.h"
#include "Quaternion.h"

// 前方宣言
struct Vec2;
struct Quaternion;

// ベクター3の構造体
struct Vec3
{
	// 三次元ベクトル
	float x; // ベクトルの x 要素
	float y; // ベクトルの y 要素
	float z; // ベクトルの z 要素

	// コンストラクタ
	constexpr Vec3() : x(0), y(0), z(0) {};
	constexpr Vec3(const float num) : x(num), y(num), z(num) {};
	constexpr Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {};

	// 静的メンバ
	const static Vec3 left;    // Vec3(-1,0,0) または単位ベクトル
	const static Vec3 right;   // Vec3(1,0,0) または単位ベクトル
	const static Vec3 up;      // Vec3(0,1,0) または単位ベクトル
	const static Vec3 down;    // Vec3(0,-1,0) または単位ベクトル
	const static Vec3 front;   // Vec3(0,0,1) または単位ベクトル
	const static Vec3 back;    // Vec3(0,0,-1) または単位ベクトル
	const static Vec3 one;     // Vec3(1,1,1) または単位ベクトル
	const static Vec3 zero;    // Vec3(0,0,0) または単位ベクトル

	float Max() const;
	float Min() const;
	float Length() const;    // ベクトルの長さ
	float LengthSq() const;  // ベクトルの長さの2乗
	Vec3 Norm() const;      // 正規化されたベクトル

	static Vec3 Lerp(const Vec3 s, const Vec3 e, const float f);

	// 静的メソッド
	static float Dot(const Vec3 v1, const Vec3 v2);    // 二つのベクトルの内積
	static Vec3 Cross(const Vec3 v1, const Vec3 v2);  // 二つのベクトルの外積
	static float Distance(const Vec3 v1, const Vec3 v2);   // 二つのベクトル間の距離
	static float DistanceSq(const Vec3 v1, const Vec3 v2); // 二つのベクトル間の距離の2乗

	// 二つのベクトルの要素ごとの最大値を返す
	static Vec3 Max(const Vec3 v1, const Vec3 v2);
	// 二つのベクトルの要素ごとの最小値を返す
	static Vec3 Min(const Vec3 v1, const Vec3 v2);

	// ベクトルの加算
	Vec3 operator+(const Vec3 other) const; // ベクトルの加算
	Vec3 operator-(const Vec3 other) const; // ベクトルの減算
	Vec3 operator*(const Vec3 other) const; // ベクトルの乗算
	Vec3 operator/(const Vec3 other) const; // ベクトルの除算
	Vec3 operator+(const Vec2 other) const; // ベクトルの加算
	Vec3 operator-(const Vec2 other) const; // ベクトルの減算
	Vec3 operator*(const Vec2 other) const; // ベクトルの乗算
	Vec3 operator/(const Vec2 other) const; // ベクトルの除算
	Vec3 operator+(float num) const;       // スカラーの加算
	Vec3 operator-(float num) const;       // スカラーの減算
	Vec3 operator*(float num) const;       // スカラーの乗算
	Vec3 operator/(float num) const;       // スカラーの除算
	Vec3 operator-() const;

	// ベクトルの加算代入
	Vec3& operator+=(const Vec3 other);
	Vec3& operator-=(const Vec3 other);
	Vec3& operator+=(float num);
	Vec3& operator-=(float num);
	Vec3& operator*=(float num);
	Vec3& operator/=(float num);

	// スカラー代入
	Vec3& operator=(float num);
	Vec3& operator=(const Vec2 other); // Vec2からVec3への変換
	Vec3& operator=(const Quaternion& q); // QuaternionからVec3への変換

	// 前置インクリメント/デクリメント
	Vec3& operator++();    // 前置インクリメント
	Vec3 operator++(int);  // 後置インクリメント
	Vec3& operator--();    // 前置デクリメント
	Vec3 operator--(int);  // 後置デクリメント

	// 比較演算子
	bool operator==(const Vec3 other);
	bool operator!=(const Vec3 other);
	bool operator>=(const Vec3 other);
	bool operator<=(const Vec3 other);
	bool operator>(const Vec3 other);
	bool operator<(const Vec3 other);
	bool operator==(const Vec2 other);
	bool operator!=(const Vec2 other);
	bool operator>=(const Vec2 other);
	bool operator<=(const Vec2 other);
	bool operator>(const Vec2 other);
	bool operator<(const Vec2 other);
	bool operator==(const float num);
	bool operator!=(const float num);
	bool operator>=(const float num);
	bool operator<=(const float num);
	bool operator>(const float num);
	bool operator<(const float num);
};
