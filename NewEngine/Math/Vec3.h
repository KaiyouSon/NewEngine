#pragma once
#include "Vec2.h"

class Vec2;

class Vec3
{
public:
	// 変数
	float x; // ベクトルの x 成分
	float y; // ベクトルの y 成分
	float z; // ベクトルの z 成分

	// コンストラクタ
	constexpr Vec3() : x(0), y(0), z(0) {};
	constexpr Vec3(const float& num) : x(num), y(num), z(num) {};
	constexpr Vec3(const float& x, const float& y, const float& z) : x(x), y(y), z(z) {};

	// static変数
	const static Vec3 left;    // Vec3(-1,0,0) と同じ意味
	const static Vec3 right;   // Vec3(1,0,0) と同じ意味
	const static Vec3 up;      // Vec3(0,1,0) と同じ意味
	const static Vec3 down;    // Vec3(0,-1,0) と同じ意味
	const static Vec3 front; // Vec3(0,0,1) と同じ意味
	const static Vec3 back;    // Vec3(0,0,-1)と同じ意味
	const static Vec3 one;     // Vec3(1,1,1) と同じ意味
	const static Vec3 zero;    // Vec3(0,0,0) と同じ意味

	// 関数
	float Lenght() const;    // ベクトルの大きさ
	Vec3 Norm() const;    // 正規化にしたベクトル

	// static関数
	static float Dot(const Vec3& v1, const Vec3& v2);		// 二つのベクトルの内積
	static Vec3 Cross(const Vec3& v1, const Vec3& v2);		// 二つのベクトルの外積
	static float Distance(const Vec3& v1, const Vec3& v2);	// 二つのベクトルの距離

	// 二つのベクトルで各成分の一番大きな値を使用してベクトルを作成する
	static Vec3 Max(const Vec3& v1, const Vec3& v2);
	// 二つのベクトルで各成分の一番小さな値を使用してベクトルを作成する
	static Vec3 Min(const Vec3& v1, const Vec3& v2);

	// 算術演算子のオーバーロード
	Vec3 operator+(const Vec3& other) const; // もう一方のベクトルとの足し算
	Vec3 operator-(const Vec3& other) const; // もう一方のベクトルとの引き算
	Vec3 operator*(const Vec3& other) const; // もう一方のベクトルとの掛け算
	Vec3 operator/(const Vec3& other) const; // もう一方のベクトルとの割り算
	Vec3 operator+(const Vec2& other) const; // もう一方のベクトルとの足し算
	Vec3 operator-(const Vec2& other) const; // もう一方のベクトルとの引き算
	Vec3 operator*(const Vec2& other) const; // 一つの値との掛け算
	Vec3 operator/(const Vec2& other) const; // 一つの値との割り算
	Vec3 operator+(float num) const;		 // 一つの値との足し算
	Vec3 operator-(float num) const;		 // 一つの値との引き算
	Vec3 operator*(float num) const;         // 一つの値との掛け算
	Vec3 operator/(float num) const;         // 一つの値との割り算
	Vec3 operator-() const;

	// 複合代入演算子のオーバーロード
	Vec3& operator+=(const Vec3& other); // 複合代入演算 +=
	Vec3& operator-=(const Vec3& other); // 複合代入演算 -=
	Vec3& operator+=(float num);         // 複合代入演算 +=
	Vec3& operator-=(float num);         // 複合代入演算 -=
	Vec3& operator*=(float num);         // 複合代入演算 *=
	Vec3& operator/=(float num);         // 複合代入演算 /=

	// 代入演算子のオーバーロード
	Vec3& operator=(float num);			// 一つの値を代入
	Vec3& operator=(const Vec2& other); // Vec2をVec3に変換

	// インクリメント/デクリメント演算子のオーバーロード
	Vec3& operator++();   // 前置インクリメント
	Vec3 operator++(int); // 後置インクリメント
	Vec3& operator--();   // 前置デクリメント
	Vec3 operator--(int); // 後置デクリメント

	// 比較演算子のオーバーロード
	bool operator==(const Vec3& other);
	bool operator!=(const Vec3& other);
	bool operator>=(const Vec3& other);
	bool operator<=(const Vec3& other);
	bool operator==(float num);
	bool operator!=(float num);
	bool operator>=(float num);
	bool operator<=(float num);
};
